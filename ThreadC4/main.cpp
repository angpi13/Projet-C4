#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <pthread.h>

#include "VehiculeCAN.h"
using namespace std;

//Variables utiles :

SNPortSerie port;  //objet pour utiliser SNPortSerie
MessageCAN tabidCAN[2048];	//Création de mon tableau qui va repertorier les id reliés aux trames
string trameInjectee;
int nbInjection;
int tempoInjection;      //Espacement temporel des inj en ms hexa
bool stopLecture;

VehiculeCAN vehicule;

void *threadLancementServeur(void * arg)
{
    //Ouverture du serveur
    ServeurTCP monserveur("0.0.0.0",2026);

    //Création de variables
    char requeteidCAN[101];
    int n;
    
    while (true)
    {
        cout << "Attente d'un client..."<<endl;
        monserveur.AttendreClient();    //méthode bloquante, qui ne va pas    
                                        //continuer tant que attendre est pas true    
        do
        {
            n = monserveur.Recevoir(requeteidCAN,100);  //méthode bloquante
            requeteidCAN[n]=0;      //si tu recois n char, tu mets 0 au nième, le dernier quoi
            cout << "[ " << n << " octets ] : " << requeteidCAN << " (requete client)" << endl;
            char reponse[10000]="ok";
            string stringRequeteidCAN=requeteidCAN;
            
            if (n < 5)  //ID demandé, 000 pour demander toutes les trames
            {
                int id=vehicule.ChaineHexaVersInt((char*)stringRequeteidCAN.substr(0,3).c_str(),3);
                cout << "ID="<<id<<endl;
                if (id>=0 && id<2048) //l'id est bien sur 11 bits
                {   
                    if (id) // l'id n'est pas 000
		            {	if (vehicule.TabidCAN(id).GetId()!="")  // l'id est présent dans le tableau
                        {
                            strcpy(reponse,vehicule.TabidCAN(id).GetTrameComplete().c_str());
                            monserveur.Envoyer(reponse,strlen(reponse));
                            cout << vehicule.TabidCAN(id).GetTrameComplete() << endl;
                        }
			            else monserveur.Envoyer("ID inconnu",10);
                    }
                    else  //l'id est 000
                    {
                        strcpy(reponse,vehicule.TabidCANComplet().c_str());
                        monserveur.Envoyer(reponse,strlen(reponse));
                        cout << vehicule.TabidCANComplet(); //<< endl;
                    }
		        }else monserveur.Envoyer("ID trop grand",13);
            }
            else    //pour une injection
            {
                vehicule.SetTrameInjectee(stringRequeteidCAN.substr(0,35));      
                //115 [ 8 octets ] : 0000000000007FD0 0A 0EB
                //0B6 [ 4 octets ] : 0800000000000000 0A 0EB
                if (stringRequeteidCAN.length()>35)
                {
                    int finNB = stringRequeteidCAN.find(" ",37);
                    if (finNB!=-1)
                    {
                        vehicule.SetNbInjection(vehicule.ChaineHexaVersInt((char*)stringRequeteidCAN.c_str()+36,2));
                        vehicule.SetTempoInjection(vehicule.ChaineHexaVersInt((char*)stringRequeteidCAN.c_str()+39,2));
                        cout << "Trame injectée : " << vehicule.TrameInjectee() << "\t" << vehicule.NbInjection() << " fois, chaque " << vehicule.TempoInjection() << "ms" << endl;
                    }
                }
                else
                {
                    vehicule.SetNbInjection(1);
                    vehicule.SetTempoInjection(0);
                }
            }
        }
        while (n > 0);
    }
}

void *threadLectureTrame(void * arg)
{
    unsigned char trame[100];
    bool premiere = true;

    while(true)
    {   
        string strame="";
        int n=0,r=0;
        do
        {   r=vehicule.Port()->recevoir(&trame[n]);
            if(trame[n]!='\r') strame+=trame[n];
        }while(trame[n++]!='\r' && r>=0);
        
        if (!premiere && !stopLecture && n>4)
        {
            //string strame="t0B680000000000007FD07474";
            string ftrame=vehicule.FormateTrame(strame);
            int id=vehicule.EnregistrerMessageCAN(strame);
            //if(id >= 0 && id < 2048) tabidCAN[id].AfficherMessageCAN();
            cout<<ftrame<<"\t"<<dec<<vehicule.TabidCAN(id).GetCompteur()<<endl;

            //Enregistrement fichier log
            ofstream fichier;
            fichier.open("Vscom.log",fstream::app);
            fichier << vehicule.currentDateTime()<<" : "<<ftrame<<"\t"<<vehicule.TabidCAN(id).GetCompteur()<<endl;
            fichier.close();

            //Petit dodo hebdomadaire
            //usleep(500000);
        }
        premiere=false;
    }
    pthread_exit(NULL);
}

void *threadInjectionMessage(void * arg)
{
    //string trame="115 [ 8 octets ] : 0000000000007FD0";
    while (true)
    {
        char trameD[100]="115 [ 8 octets ] : 0000000000007FD0";
        if (vehicule.TrameInjectee()!="")
        {
            strcpy(trameD,vehicule.DeFormateTrame(vehicule.TrameInjectee()).c_str());
            //cout << "Trame envoyee : "<< trameD << endl;
            strcat(trameD,"\r");
            bool r;

            for (int i=0; i<vehicule.NbInjection(); i++)
            {   for (int n=0; n<strlen(trameD); n++)
                {
                    r=vehicule.Port()->envoyer((unsigned char)trameD[n]);
                    //cout<<trameD[n]<<""<<n<<endl;
                }
                cout<<"Envoi "<< i+1 <<" de la trame : "<<trameD<<endl;
                usleep(vehicule.TempoInjection()*1000);    
            }
            vehicule.SetTrameInjectee("");
        }
    }
    pthread_exit(NULL);
}

void *threadEnregistrementBDD(void * arg)
{
    while(true)
    {
        stopLecture = true;
        bool premiere = true;

        ofstream fichier;
        fichier.open("CAN.json");
        
        string dateTime = vehicule.currentDateTime();
        
        fichier << "{ \"MessageCANRecu\":[";
        
        for (int i = 0; i < 2048; i++)
        {
            if (vehicule.TabidCAN(i).GetId()!="")
            {
                if (!premiere)
                {
                    fichier << ",\n";
                }
                premiere = false;
                
                fichier << "{\"idCAN\" : \"" << vehicule.TabidCAN(i).GetId() << "\",\n";
                fichier << "\"donnees\" : \"" << vehicule.TabidCAN(i).GetDonnee() << "\",\n";
                fichier << "\"longueur\" : " << vehicule.TabidCAN(i).GetLongueur() << ",\n";
                fichier << "\"compteur\" : " << vehicule.TabidCAN(i).GetCompteur() << ",\n";
                fichier << "\"dateHeure\" : \"" << dateTime << "\",\n";
                fichier << "\"id_voiture\" : " << 1 << "}";
            }            
        }
        fichier << "\n]\n}";
        
    
        fichier.close();
        stopLecture = false;

        ClientTCP clientBdd;
        clientBdd.SeConnecterAUnServeur("172.20.21.21",80);
        
        string requete = "POST /projetC4/restC4.php/MessageCANRecu HTTP/1.1\r\nHost: 172.20.21.21\r\nConnexion: keep-alive\r\nContent-type: application/json\r\nContent-Length: 10127\r\n\r\n";
        string json;

        std::ifstream file("CAN.json",std::ifstream::in);        //ouvrir fichier
        std::stringstream buffer;                       //creation objet

        buffer << file.rdbuf();     //insérer une sortie formatée
        json = buffer.str();

        //fichier.read("CAN.json")
        clientBdd.Envoyer(requete + json);

        //Petit dodo hebdomadaire
        usleep(100000);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t t_serv;
    pthread_create(&t_serv, NULL, threadLancementServeur, NULL);

    cout<<"Lancement du ProjetC4 : "<<endl<<endl;
    vehicule.OuvrirVSCOM(vehicule.Port());

    string methode;
    
    cout << "Que souhaitez-vous faire ?  [lire] : 'l' ou [ecrire] : 'e' ";
    cin >> methode ;

    if (methode[0]=='l')
    {
        stopLecture = false;
        pthread_t t_lecture;        //Variable qui va contenir le thread
        pthread_create(&t_lecture, NULL, threadLectureTrame, NULL);

        pthread_t t_save;           //Variable qui va contenir le thread
        pthread_create(&t_save, NULL, threadEnregistrementBDD, NULL);    
    }
    else if(methode[0]=='e')
    {
        vehicule.SetTrameInjectee("");
        pthread_t t_inj;        //Variable qui va contenir le thread
        pthread_create(&t_inj, NULL, threadInjectionMessage, NULL);
    }
    while (true);    

    return 0;
}