#include <string>
#include <iostream>
using namespace std;

class MessageCAN
{
    private :

        string ID;
        int longueur;
        string donnee;
        int compteur;
        string trameComplete;

    public :

        MessageCAN();
        void SetID(string i);
        void SetLongueur(int l);
        void SetDonnee(string d);
        void IncrementCompteur();
        void AfficherMessageCAN();
	int  GetCompteur();

        string GetTrameComplete();
        void SetTrameComplete(string trame);
        string GetId();
        int GetLongueur();
        string GetDonnee();
};