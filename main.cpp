#include <iostream>
#include <string>
#include <unistd.h>
#include "ClientTCP.h"
#include "ServeurTCP.h"
#include "SNPortSerie.h"

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

int main()
{   SNPortSerie port;
    cout<<"Lancement du ProjetC4 : "<<endl<<endl;
   
    int ouverture = port.Ouvrir("/dev/ttyUSB0");
    if(!port.ModifierVitesse(V3000000_BAUD))
    {
	    cout << "Erreur lors du parametrage de la vitesse" << endl;
    }

    //Verification de la connexion au port COM
    if (ouverture == 0)
    {
        cout << "Ouverture port : OK" << endl;
    }
    else{
        cout << "La connexion au port serie a echouee. Veuillez verifier que le port est bien connecte a  la raspberry et non autre part..." << endl;
    }
     
////////////////////Envoi de différentes variables pour configurer le port en bon et due forme/////////////////////

    EnvoyerConfig(&port,"CFFFFFF\r",1);
    EnvoyerConfig(&port,"CFFFFFF\r",1);
    EnvoyerConfig(&port,"CFFFFFF\r",1);
    EnvoyerConfig(&port,"S4\r",1);
    EnvoyerConfig(&port,"Z1\r",1);
    EnvoyerConfig(&port,"O\r",0);
    while(true)
     {    unsigned char trame[50];
          string strame="";
          int n=0,r;
          do
          {	r=port.recevoir(&trame[n]);
                cout<<(int)trame[n]<<" ";
                strame+=trame[n];
          }while(trame[n++]!='\r' &&r>=0);
          cout<<endl<<"\t\t"<<strame<<endl;
          usleep(500000);
     }
     port.Fermer();
     
    //DODO INTERDIT !!!!!!!!!!!!!!
    
    return 0;
}

