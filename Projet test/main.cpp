#include "SNPortSerie.h"

#ifdef __unix__
#include <unistd.h>
void Sleep(int ms) { usleep(ms*1000)}
#endif

using namespace std;

VSCOM::VSCOM(){
    ouvert = false; conecteVSCOM=false;
}
void VSCOM::ModifierCOM(string scom){
    com=scom;
}
bool VSCOM::OuvrirCOM(){

        #ifdef __unix__
        portCom = new SNPortSerie();
        if(portCom->Ouvrir(com.c_str()) !=0)
        {
            cout << "Erreur lors de l'ouverture du port serie" << endl;
            return false;
        }
        if (!portCom->ModifierVitesse(V3000000_BAUD))
        {
            cout << "Erreur lors du parametrage de la vitesse" << endl;
            return false;
        }
        ouvert = true;
        cout << "Ouverture effectuée ... " << endl;
        #elif _WIN32 || _WIN64
        portCom = new Rs232((char*)com.c_str());
        portCom->configurer(CBR_9600,8,NOPARITY,ONESTOPBIT);
        portCom->ouvrir();
        ouvert = true;
        #endif
        
}