// version pour Xe6
//////////////////////////////////////////////////////////////////////////////////////////////////
//Inclure ce fichier header : #include<PortCom.h>												//
//Exemple d'utilisation de la classe IRPortSerie :													//
//  IRPortSerie *PortCom1; 					//déclaration d'un pointeur sur portcom					//
//	PortCom1 = new IRPortSerie("COM1"); 		//création de l’objet portcom		9600    8N1			//
//	PortCom1->configurer(CBR_9600,8,NOPARITY,ONESTOPBIT)										//
//	PortCom1->ouvrir();																			//
//	if (PortCom1->envoyer(0x02)!=-1)	//envoi du caractère 0x02 et test du port COM1			//
//	{	PortCom1->envoyer(0x30);		//envoi du caractère '0'								//
//		PortCom1->fermer();																		//
//	}																							//
//	delete PortCom1; 				//détruit notre objet portcom								//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef IRPortSerie_H
#define IRPortSerie_H
//#include <Classes.hpp>
#include <windows.h>
#include <winbase.h>
#include <string>
using namespace std;
class IRPortSerie
{	private:
			HANDLE Hportcom;
			DCB ConfigDCB;
			DWORD Noctlus;
			BYTE Parite,Nbstop,Nbbits;
			DWORD Vitesse;

	public:
			string  Nomport;
			IRPortSerie(string nomport);
			void configurer(DWORD vitesse,BYTE nbbits,BYTE parite,BYTE nbstop);
			void ouvrir(void);
			int envoyer (char);
			int recevoir(char*);
			void fermer(void);
};
#endif
