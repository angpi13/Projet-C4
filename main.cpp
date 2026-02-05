#include <iostream>
#include <string>
#include <unistd.h>
#include "ClientTCP.h"
#include "ServeurTCP.h"
#include "SNPortSerie.h"
using namespace std;

int main()
{
    cout<<"Lancement du ProjetC4 : "<<endl<<endl;

    SNPortSerie port;
    int open = port.Ouvrir("/dev/ttyUSB0");
    if(!port.ModifierVitesse(V3000000_BAUD))
    {
	cout << "Erreur lors du paramétrage de la vitesse" << endl;
     }


    //Vérification de la connexion au port COM
    if (open == 0)
    {
        cout << "Ouverture port : OK" << endl;
    }
    else if (open <= 0)
    {
        cout << "La connexion au port série a échouée. Veuillez vérifier que le port est bien connecté à la raspberry et non autre part..." << endl;
    }
     
////////////////////Envoi de différentes variables pour configurer le port en bon et due forme/////////////////////

    /*unsigned char c[10];
    port.envoyer('v\r');
    port.envoyer('\r');
    usleep(500000);for(int i=0;i<7;i++) port.recevoir(&c[i]);
    c[7] = 0; cout << "MSG : " << c << endl;*/
    //Demande de connexion avec envoie de variable

    const char trame[50] = "v\r"; 
    char message[501]; 

    bool send = port.EnvoyerDesOctets(trame,2);
    usleep(500000);
    int n = port.LireDesOctets(message,500);
    if (send == true) {
        cout << "le message reçu :" << message << endl;
    }
    else if (send == false)
    {
        cout << "L'envoi de la trame de connexion a échoué !!!!" <<endl;
    }
    
    cout << "Le nombre d'octets reçu : " << n << endl;
    cout << message << endl;
    
    //copie de chaîne de char sur ma variable trame
    strcpy (trame,"CFFFFFF\r");

    send = port.EnvoyerDesOctets(trame,8);
    n = port.LireDesOctets(message,500);
    if (send == true) {
        cout << "le message reçu :" << message << endl;
    }
    else if (send == false)
    {
        cout << "L'envoie de la trame de connexion a échoué !!!!" <<endl;
    }
    


    port.Fermer();

    























                //DODO INTERDIT !!!!!!!!!!!!!!
//     IRPortSerie *PortCom; 					//déclaration d'un pointeur sur portcom					//
// 	PortCom = new IRPortSerie("COM15"); 		//création de l’objet portcom		9600    8N1			//
// 	PortCom->configurer(CBR_9600,8,NOPARITY,ONESTOPBIT);
// 	PortCom->ouvrir();
// //    if (pdo==true) cout << "good"<<endl;
//     Sleep(1000);

//     while(true)
//     {   char chaine[100],hexdec[10];
// 	    string trame="";
//         char c;

//         int n=PortCom->recevoir1car(&c);

// 			// Dans chaine il y a les n valeurs
//             for(int i=0;i<n;i++)
//             {   sprintf(hexdec,"%.2X ",chaine[i]&0xff);
//             	trame+=hexdec;
//             }
//             if(n) cout<<n<<" : "<<trame<<endl;                      //Pour afficher la code hexa
//             //float vitesse=chaine[4]+((float)chaine[5]/100);
//         //if(n)
//         cout<<chaine<<endl;
//         //cout<<n<<endl;
//         //cout<<"vaaaaazy"<<endl;
//     	Sleep(1000);

//     }
    return 0;
}
