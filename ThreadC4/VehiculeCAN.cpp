#include "VehiculeCAN.h"

VehiculeCAN::VehiculeCAN()
{
    trameInjectee="";
    nbInjection=1;
    tempoInjection=0;
}

const string VehiculeCAN::currentDateTime()
{   time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = * localtime( & now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", & tstruct);
    return buf;
}

unsigned int VehiculeCAN::ChaineHexaVersInt(char *chaineHexa, int nbCar)
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

string VehiculeCAN::TabidCANComplet()
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
void VehiculeCAN::EnvoyerConfig(SNPortSerie *portSerie,char *config, int nbOctetsAttendus)
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
string VehiculeCAN::FormateTrame(string trame){
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
string VehiculeCAN::DeFormateTrame(string trame){
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

int VehiculeCAN::EnregistrerMessageCAN(string message)
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

bool VehiculeCAN::OuvrirVSCOM(SNPortSerie *portSerie)
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
        cout << "La connexion au port serie a echouee. Veuillez verifier que le port est bien connecte a� la raspberry et non autre part..." << endl;
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

MessageCAN VehiculeCAN::TabidCAN(int id)
{
    return tabidCAN[id];
}

SNPortSerie* VehiculeCAN::Port()
{
    return &port;
}

string VehiculeCAN::TrameInjectee()
{
    return trameInjectee;
}

int VehiculeCAN::NbInjection()
{
    return nbInjection;
}

int VehiculeCAN::TempoInjection()
{
    return tempoInjection;
}

void VehiculeCAN::SetTrameInjectee(string trame)
{
    trameInjectee = trame;
}

void VehiculeCAN::SetNbInjection(int nb)
{
    nbInjection = nb;
}

void VehiculeCAN::SetTempoInjection(int tempo)
{
    tempoInjection = tempo;
}