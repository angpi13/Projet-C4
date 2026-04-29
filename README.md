# Projet-C4

Le bus de données CAN, de son diminutif "Controller Area Network", est un bus système série très répandu dans beaucoup d'industries, notamment l'automobile.

Lors de la communication avec la maquette Citroën C4, nous utilisons un VSCOM qui permet la convertion CAN-USB. Ainsi, via cet adaptateur, nous recevons les messages émis de la maquettes et nous pouvont de même
en émettre vers celle-ci.

# La Messagerie
Via l'adaptateur, les messages se présentent sous la forme suivante : 
<img width="1920" height="198" alt="image" src="https://github.com/user-attachments/assets/11e24a04-3af4-4051-a9de-7e3c8184c5b0" />
autrement écrit : t0B680000000000007FD07474

Chaque octet, chaque caractère à une signification bien précise :

  t ou r(requête)		: standard (T ou R majuscule pour extended)
  0B6			          : identifiant
  8			            : DLC	
  0000000000007FD0 	: Donnée
  7474			: CRC

►t ou r correspond au **type** de requete
►l'identifiant définit l’**identificateur** du message
►le DLC correspond à **la longueur** en octets du champs de données (suivant) du message
►donnée comporte les **données utiles**. Taille maximale = 8 octets
►le CRC : Séquence CRC sur 15 bits à Bit délimiteur de CRC qui est toujours récessif
