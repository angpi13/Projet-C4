#include "IRPortSerie.h"
// version pour Xe6

IRPortSerie::IRPortSerie(string nomport)
{ 	configurer(CBR_9600,8,NOPARITY,ONESTOPBIT);
	if(nomport.length() >4)
		Nomport = "\\\\.\\" + nomport;
	else
		Nomport = nomport;
}


void IRPortSerie::configurer(DWORD vitesse,BYTE nbbits,BYTE parite,BYTE nbstop)
{ 	Vitesse=vitesse;
	Nbbits=nbbits;
	Nbstop=nbstop;
	Parite=parite;
}
void IRPortSerie::ouvrir(void)
{ 	Hportcom= CreateFile(Nomport.c_str(),/*fdwAccess*/GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	GetCommState(Hportcom, &ConfigDCB);
	ConfigDCB.BaudRate=Vitesse;
	ConfigDCB.Parity=Parite;
	ConfigDCB.StopBits=Nbstop;
	ConfigDCB.ByteSize=Nbbits;
	SetCommState(Hportcom, &ConfigDCB);
	COMMTIMEOUTS ct;
	memset ((void *) &ct, 0, sizeof (ct));
	ct.ReadIntervalTimeout = MAXDWORD;
	ct.ReadTotalTimeoutMultiplier = 0;
	ct.ReadTotalTimeoutConstant = 0;
	ct.WriteTotalTimeoutMultiplier = 20000L / Vitesse;
	ct.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts (Hportcom, &ct);
}
void IRPortSerie::fermer(void)
{ 	CloseHandle(Hportcom);
}
int IRPortSerie::envoyer(char c)
{ 	DWORD Nbecrit; DWORD lg=1;
	if(WriteFile( Hportcom,&c,lg,&Nbecrit, NULL)==0) return -1;
	if(Nbecrit == lg) return 0;
	return -1;
}
int IRPortSerie::recevoir(char* c)
{   DWORD Nlus; ReadFile(Hportcom,(LPVOID *)c, 1, &Nlus, NULL) ;
	if (Nlus == 1) return 0;
	return -1;
}
int IRPortSerie::recevoir1car(unsigned char* c)
{   DWORD Nlus=0; ReadFile(Hportcom,(LPVOID *)c, 1, &Nlus, NULL);
    if(Nlus==1) return 0;
	return -1;
}

