CC=g++
CFLAGS=-c -w


#Création des fichiers obj
all : thread main.o

#Exécution main
main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

#Exécution ClientTCP
ClientTCP.o : ClientTCP.h ClientTCP.cpp
	$(CC) $(CFLAGS) ClientTCP.cpp

#Exécution ServeurTCP
Serveur.o : Serveur.h Serveur.cpp
	$(CC) $(CFLAGS) Serveur.cpp

#Exécution SNPortSerie
SNPortSerie.o : SNPortSerie.h SNPortSerie.cpp
	$(CC) $(CFLAGS) SNPortSerie.cpp

#Exécution MessageCAN
MessageCAN.o : MessageCAN.h MessageCAN.cpp
	$(CC) $(CFLAGS) MessageCAN.cpp

#Excécution VehiculeCAN
VehiculeCAN.o : VehiculeCAN.h VehiculeCAN.cpp
	$(CC) $(CFLAGS) VehiculeCAN.cpp

#Exécution finale
thread : main.o ClientTCP.o ServeurTCP.o SNPortSerie.o MessageCAN.o VehiculeCAN.o
	$(CC) main.o ClientTCP.o ServeurTCP.o SNPortSerie.o MessageCAN.o VehiculeCAN.o

#Clean
clean :
	rm -f *.o thread

