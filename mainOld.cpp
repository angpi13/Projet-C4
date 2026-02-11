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
*/


    port.Fermer();

                //DODO INTERDIT !!!!!!!!!!!!!!
    return 0;
}
