#include <iostream>
#include <string>
#include <unistd.h>
#include "ClientTCP.h"
#include "ServeurTCP.h"
#include "SNPortSerie.h"
using namespace std;

//Création de la methode qui permet de configurer le port COM :
void EnvoyerConfig(SNPortSerie portSerie,char *config, int nbOctets)
{   char reponse[501]; 
    bool envoi = portSerie.EnvoyerDesOctets(config,nbOctets);
    usleep(500000);
    int n = portSerie.LireDesOctets(reponse,500);
    if (envoi) 
    {   cout << "le message reçu : " << reponse << endl;
    }
    else 
    {   cout << "L'envoi de la trame de connexion a échoué !!!!" <<endl;
    }
    usleep(500000);
} 

int main()
{
    cout<<"Lancement du ProjetC4 : "<<endl<<endl;

    SNPortSerie port;
    int ouverture = port.Ouvrir("/dev/ttyUSB0");
    if(!port.ModifierVitesse(V3000000_BAUD))
    {
	    cout << "Erreur lors du paramétrage de la vitesse" << endl;
    }

    //Vérification de la connexion au port COM
    if (ouverture == 0)
    {
        cout << "Ouverture port : OK" << endl;
    }
    else{
        cout << "La connexion au port série a échouée. Veuillez vérifier que le port est bien connecté à la raspberry et non autre part..." << endl;
    }
     
////////////////////Envoi de différentes variables pour configurer le port en bon et due forme/////////////////////

    // EnvoyerConfig(port,"v\r",2);
    // EnvoyerConfig(port,"CFFFFFF\r",8);
    // EnvoyerConfig(port,"CFFFFFF\r",8);
    // EnvoyerConfig(port,"CFFFFFF\r",8);
    // EnvoyerConfig(port,"S4\r",3);
    // EnvoyerConfig(port,"Z1\r",3);
    // EnvoyerConfig(port,"L\r",2);


    /*unsigned char c[10];
    port.envoyer('v\r');
    port.envoyer('\r');
    usleep(500000);for(int i=0;i<7;i++) port.recevoir(&c[i]);
    c[7] = 0; cout << "MSG : " << c << endl;*/
    //Demande de connexion avec envoie de variable

    char trame[50] = "v\r"; 
    char message[501]; 

    bool send = port.EnvoyerDesOctets(trame,2);
    usleep(500000);
    int n = port.LireDesOctets(message,500);
    if (send == true) {
        cout << n<<" le message reçu de vr :" << message << endl;
    }
    else{
        cout << "L'envoi de la trame de connexion a échoué !!!!" <<endl;
    }
    
    //cout << "Le nombre d'octets reçu : " << n << endl;
    //cout << message << endl;
    
    //copie de chaîne de char sur ma variable trame
/*    strcpy (trame,"CFFFFFF\r");

    send = port.EnvoyerDesOctets(trame,8);
    usleep(500000);
    n = port.LireDesOctets(message,500);
    message[n]=0;
    if (send == true) {
        cout << n<< " le message reçu de "<<trame<< " : " << message << endl;
    }
    else{
        cout << "L'envoi de la trame de connexion a échoué !!!!" <<endl;
    }
  */
    strcpy (trame,"CFFFFFF\r");
    for (int i=0;i<strlen(trame);i++) port.envoyer(trame[i]);
    //send = port.EnvoyerDesOctets(trame,8);
    usleep(500000);
    //n = port.LireDesOctets(message,500);
    //message[n]=0;
    unsigned char c[10];
    port.recevoir(&c[0]);
    cout<<(int)c[0]<<endl;
    if (send == true) {
        cout << n<<" le message reçu de "<<trame<<" : " << message << endl;
    }
    else{
        cout << "L'envoi de la trame de connexion a échoué !!!!" <<endl;
    }
    


    //Réception et affichage des trames sur le format " idCAN [ n octets ] : 0000000000000000 compteur "

/*    char donnees[100];
    if (config ==  true)
    {
        while (true)
        {
            port.LireDesOctets(donnees,100);
            cout<<donnees<<endl;
            usleep(100000);
        }
        
    }

        string type = strame.substr (0,1);                  // "t"
        string idCAN = strame.substr(1,3);                  //"idCAN : 0B6"
        string DLC = strame.substr(4,1);                    //DLC : taille de la trame en octets
        string donnees = strame.substr(5,16);               //donnees
        string CRC = strame.substr(21,4);        //Check sum ?
        
        string Type_CAN = type + idCAN;

        //cout << type << ' ' << endl << idCAN << ' ' << endl << DLC << ' '<< endl << donnees << ' '<< endl << CRC << ' '<< endl;
        string trame_rep = " " + idCAN + " [ " + DLC + " octets ] : " + donnees + "  " + "compteur";
        
        cout << trame_rep << "   " << Type_CAN << endl;
*/

//  do 
    // {
    //     //On demande a l'utilisateur
    //     cout << "Sous quel format souhaitez-vous visionner les trames ?  [SerialPort][Muxtrace]" << endl;
    //     cin >> methode ;
    //     cin.get();


    //     //On receptionne les trames
    //     unsigned char trame[50];
    //     string strame="";
    //     int n=0,r;
    //     do
    //     {   r=port.recevoir(&trame[n]);
    //         cout<<(int)trame[n]<<" ";
    //         strame+=trame[n];
    //     }while(trame[n++]!='\r' &&r>=0);


    //     //if POUR AFFICHAGE SERIAL PORT
    //     if (methode == "SerialPort")
    //     {
    //         while (true){
    //         cout<<endl<<"\t\t"<<strame<<endl;
    //         usleep(500000);
    //         }
    //     }
    //     //else if POUR AFFICHAGE MUXTRACE
    //     else if (methode == "Muxtrace")
    //     {
    //         while (true){
    //         cout << "Trame : ";
    //         FormateTrame(strame);

    //         }
    //     }
    //     else (cout << "La bonne commande n'a pas été rentrée, veuillez reessayer");



    // } while (methode == "SerialPort" || methode == "Muxtrace");
    port.Fermer();

                //DODO INTERDIT !!!!!!!!!!!!!!
    return 0;
}
