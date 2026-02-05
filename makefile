CC=g++ 
CFLAGS=-c -w
# CFLAGS=-Wall -c
#set PATH= %PATH%;C:\msys64\ucrt64\bin\g++

# Création des fichiers objets pour faire les commandes après
all : serveur main.o ClientTCP.o ServeurTCP.o SNPortSerie.o

# Exécution du main.cpp
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

# Exécution de ClientTCP
ClientTCP.o : ClientTCP.h ClientTCP.cpp
	$(CC) $(CFLAGS) ClientTCP.cpp 

# Exécution de ServeurTCP
ServeurTCP.o : ServeurTCP.h ServeurTCP.cpp
	$(CC) $(CFLAGS) ServeurTCP.cpp

# Exécution de SNPortSerie
SNPortSerie.o : SNPortSerie.h SNPortSerie.cpp
	$(CC) $(CFLAGS) SNPortSerie.cpp

# Execution finale
serveur : main.o ClientTCP.o ServeurTCP.o SNPortSerie.o
	$(CC) main.o ClientTCP.o ServeurTCP.o SNPortSerie.o -o serveur

#Nettoyer le projet
clean : 
	rm -f *.o serveur
