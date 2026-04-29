
#include "MessageCAN.h"

MessageCAN::MessageCAN()
{
    ID="";
    longueur=0;
    donnee="";
    compteur=0;
}

void MessageCAN::SetID(string i)
{
    ID=i;
}

void MessageCAN::SetLongueur(int l)
{
    longueur=l;
}

void MessageCAN::SetDonnee(string d)
{
    donnee=d;
}

void MessageCAN::IncrementCompteur()
{
    compteur++;
}

void MessageCAN::AfficherMessageCAN()
{
    cout << ID <<"\t[ "<< longueur <<" ]\t"<<donnee <<"\t"<< compteur <<endl;
}

int  MessageCAN::GetCompteur()
{	
    return compteur;
}

string MessageCAN::GetTrameComplete()
{
    return trameComplete;
}

void MessageCAN::SetTrameComplete(string trame)
{
    trameComplete=trame;
}

string MessageCAN::GetId()
{
    return ID;
}

int MessageCAN::GetLongueur()
{
    return longueur;
}

string MessageCAN::GetDonnee()
{
    return donnee;
}
