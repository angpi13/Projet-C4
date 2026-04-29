#include <fstream>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <pthread.h>

#include "ClientTCP.h"
#include "ServeurTCP.h"
#include "SNPortSerie.h"
#include "MessageCAN.h"
using namespace std;

//Variables utiles :

SNPortSerie port;  //objet pour utiliser SNPortSerie
MessageCAN tabidCAN[2048];	//Création de mon tableau qui va repertorier les id reliés aux trames
string trameInjectee;
int nbInjection;
int tempoInjection;      //Espacement temporel des inj en ms hexa

const string currentDateTime()
{   time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = * localtime( & now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", & tstruct);
    return buf;
}

unsigned int ChaineHexaVersInt(char *chaineHexa, int nbCar)
{   int nombre=0,j;
	char caractere;
	for(j=0;j<nbCar;j++)
	{   caractere=chaineHexa[j];
		if(caractere>='0' && caractere<='9')
		{  nombre=nombre+((caractere-'0')<<(4*(nbCar-j-1)));
		}
		else nombre=nombre+((caractere-'A'+10)<<(4*(nbCar-j-1)));
	}
	return nombre;
}

string TabidCANComplet()
{
    string tabidComplet="";
    for (int i = 1; i < 2048; i++)
    {
        if (tabidCAN[i].GetId().length()!=0);
            tabidComplet+=tabidCAN[i].GetTrameComplete()+"\n";   
    }
    return tabidComplet;         
}

//Creation de la methode qui permet de configurer le port COM :
void EnvoyerConfig(SNPortSerie *portSerie,char *config, int nbOctetsAttendus)
{   unsigned char reponse[10]; 
    for(int i=0;i<strlen(config);i++) 
    {	portSerie->envoyer(config[i]);
        cout<<hex<<(int)config[i]<<" ";
    }
    cout<<endl;
    usleep(500000);
    for(int i=0;i<nbOctetsAttendus;i++) 
    {	portSerie->recevoir(&reponse[i]);
        cout<<hex<<(int)reponse[i]<<" ";
    }
    cout<<endl;
}

//Réception et affichage des trames à partir du format "t0B680000000000007FD07474"
string FormateTrame(string trame){
    if(trame.length()>9)
    {	string idCAN = trame.substr(1,3);
	string DLC = trame.substr(4,1);
	string donnees = trame.substr(5,trame.length()-9);
	//cout << idCAN << " [ " << DLC << " octets ] : " << donnees << endl << endl ;
	return idCAN + " [ " + DLC + " octets ] : " + donnees;
    }
    return "";
}

//Réception et affichage des trames à partir du format " idCAN [ n octets ] : 0000000000000000 compteur "
string DeFormateTrame(string trame){
    if(trame.length()>=35)
    {	string idCAN = trame.substr(0,3);
	string DLC = trame.substr(6,1);
	int longueur=DLC[0]-'0';
	//cout<<longueur<<endl;
	string donnees = trame.substr(19,trame.length()-19-16+2*longueur);
        //cout << idCAN << DLC << donnees << endl << endl ;
        return "t" + idCAN + DLC + donnees;
    }
    return "";
}

int EnregistreMessageCAN(string message)
{  
    int id=0;
    if (message.length()>=5)
    {   
        id=ChaineHexaVersInt((char*)message.substr(1,3).c_str(),3);
        if (id>0 && id<2048)
        {
            tabidCAN[id].SetID(message.substr(1,3));
            int l=ChaineHexaVersInt((char*)message.substr(4,1).c_str(),1);
            tabidCAN[id].SetLongueur(l);
            tabidCAN[id].SetDonnee(message.substr(5,2*l));
            tabidCAN[id].IncrementCompteur();
            tabidCAN[id].SetTrameComplete(FormateTrame(message));    
        
        } else return 0;
    } return id;
}

bool OuvrirVSCOM(SNPortSerie *portSerie)
{
    int ouverture = portSerie->Ouvrir("/dev/ttyUSB0");
    if(!portSerie->ModifierVitesse(V3000000_BAUD))
    {
	    cout << "Erreur lors du parametrage de la vitesse" << endl;
    }

    //Verification de la connexion au port COM
    if (ouverture == 0)
    {
        cout << "Ouverture port : OK" << endl;
    }
    else{
        cout << "La connexion au port serie a echouee. Veuillez verifier que le port est bien connecte a  la raspberry et non autre part..." << endl;
    }
     
////////////////////Envoi de differentes variables pour configurer le port en bon et due forme/////////////////////

    EnvoyerConfig(portSerie,"CFFFFFF\r",1);
    EnvoyerConfig(portSerie,"CFFFFFF\r",1);
    EnvoyerConfig(portSerie,"CFFFFFF\r",1);
    EnvoyerConfig(portSerie,"S4\r",1);
    EnvoyerConfig(portSerie,"Z1\r",1);
    EnvoyerConfig(portSerie,"O\r",0);

    cout << "Le VSCOM a bien ete configuré !" << endl;
    return true;
}


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
                int id=ChaineHexaVersInt((char*)stringRequeteidCAN.substr(0,3).c_str(),3);
                cout << "ID="<<id<<endl;
                if (id>=0 && id<2048) //l'id est bien sur 11 bits
                {   
                    if (id) // l'id n'est pas 000
		            {	if (tabidCAN[id].GetId()!="")  // l'id est présent dans le tableau
                        {
                            strcpy(reponse,tabidCAN[id].GetTrameComplete().c_str());
                            monserveur.Envoyer(reponse,strlen(reponse));
                            cout << tabidCAN[id].GetTrameComplete() << endl;
                        }
			            else monserveur.Envoyer("ID inconnu",10);
                    }
                    else  //l'id est 000
                    {
                        strcpy(reponse,TabidCANComplet().c_str());
                        monserveur.Envoyer(reponse,strlen(reponse));
                        cout << TabidCANComplet(); //<< endl;
                    }
		        }else monserveur.Envoyer("ID trop grand",13);
            }
            else    //pour une injection
            {
                trameInjectee=stringRequeteidCAN.substr(0,35);      
                //115 [ 8 octets ] : 0000000000007FD0 0A 0EB
                //0B6 [ 4 octets ] : 0800000000000000 0A 0EB
                if (stringRequeteidCAN.length()>35)
                {
                    nbInjection=ChaineHexaVersInt((char*)stringRequeteidCAN.c_str()+36,2);
                    tempoInjection=ChaineHexaVersInt((char*)stringRequeteidCAN.c_str()+39,2);
                    cout << "Trame injectée : " << trameInjectee << "\t" << nbInjection << " fois, chaque " << tempoInjection << "ms" << endl;
                }
                else
                {
                    nbInjection=1;
                    tempoInjection=0;
                }
            }
        }
        while (n > 0);
    }
}

void *threadLectureTrame(void * arg)
{
    unsigned char trame[99];
    bool premiere;

    while(true)
    {   
        string strame="";
        int n=0,r=0;
        do
        {   r=port.recevoir(&trame[n]);
            if(trame[n]!='\r') strame+=trame[n];
        }while(trame[n++]!='\r' && r>=0);
        
        if (!premiere)
        {
            //string strame="t0B680000000000007FD07474";
            string ftrame=FormateTrame(strame);
            int id=EnregistreMessageCAN(strame);
            //if(id >= 0 && id < 2048) tabidCAN[id].AfficherMessageCAN();
            //cout<<ftrame<<"\t"<<dec<<tabidCAN[id].GetCompteur()<<endl;

            //Enregistrement fichier log
            ofstream fichier;
            fichier.open("Vscom.log",fstream::app);
            fichier << currentDateTime()<<" : "<<ftrame<<"\t"<<tabidCAN[id].GetCompteur()<<endl;
            fichier.close();
            
            //Petit dodo hebdomadaire
            usleep(500000);
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
        if (trameInjectee!="")
        {
            strcpy(trameD,DeFormateTrame(trameInjectee).c_str());
            cout << "Trame envoyee : "<< trameD << endl;
            strcat(trameD,"\r");
            bool r;

            for (int i=0; i<nbInjection; i++)
            {   for (int n=0; n<strlen(trameD); n++)
                {
                    r=port.envoyer((unsigned char)trameD[n]);
                    //cout<<trameD[n]<<""<<n<<endl;
                }
                cout<<"Envoi "<< i+1 <<" de la trame : "<<trameD<<endl;
                usleep(tempoInjection*1000);    
            }
            trameInjectee="";
        }
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t t_serv;
    pthread_create(&t_serv, NULL, threadLancementServeur, NULL);

    cout<<"Lancement du ProjetC4 : "<<endl<<endl;
    OuvrirVSCOM(&port);

    string methode;
    
    cout << "Que souhaitez-vous faire ?  [lire] : 'l' ou [ecrire] : 'e' ";
    cin >> methode ;

    if (methode[0]=='l')
    {
        pthread_t t_lecture;        //Variable qui va contenir le thread
        pthread_create(&t_lecture, NULL, threadLectureTrame, NULL);
    }
    else if(methode[0]=='e')
    {
        trameInjectee="";
        pthread_t t_inj;        //Variable qui va contenir le thread
        pthread_create(&t_inj, NULL, threadInjectionMessage, NULL);
    }
    while (true);    

    return 0;
}
