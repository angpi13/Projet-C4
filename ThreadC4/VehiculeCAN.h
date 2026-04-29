#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "ClientTCP.h"
#include "ServeurTCP.h"
#include "SNPortSerie.h"
#include "MessageCAN.h"
using namespace std;

class VehiculeCAN
{
    private :

        SNPortSerie port;
        MessageCAN tabidCAN[2048];
        string trameInjectee;
        int nbInjection;
        int tempoInjection;
    
    public : 

        VehiculeCAN();
        const string currentDateTime();
        unsigned int ChaineHexaVersInt(char *chaineHexa, int nbCar);
        string TabidCANComplet();
        void EnvoyerConfig(SNPortSerie *portSerie, char *config, int nbOctetsAttendus);
        string FormateTrame(string trame);
        string DeFormateTrame(string trame);
        int EnregistrerMessageCAN(string message);
        bool OuvrirVSCOM(SNPortSerie *portSerie);

        MessageCAN TabidCAN(int id);
        SNPortSerie* Port();

        string TrameInjectee();
        int NbInjection();
        int TempoInjection();
        void SetTrameInjectee(string trame);
        void SetNbInjection(int nb);
        void SetTempoInjection(int tempo);

};