Ji Stéphane 
Huang Yvonne 
Bakalov Alexander

# Réalisation DMA pour l'IA 

Objectif : Liberer le CPU des tâches répétitives de transfert de données.

point-clé  optimisation des mouvements de données


## Hypothèse création structure logiciel de DMA 

### Liste d'attente pour les jobs 

Tableau fixe et physique de taille de jobs. 
4 pointeurs : 
    - Liste case du tableau vide, 
    - Liste case contenant job non traité par le DMA
    - Liste case contenant job en cours de traitement par le DMA
    - Liste case contenant job qui ont fini de traité (voir comment les cases sont désalloués)

### Structure d'un job 

src_deb, src_end, trg_src, trg_end, lock_read, lock_write, taille, id  
logiquement : 8 element * 1 mot = 8 mots, un job a besoin de 8 mots. 


voir comment résoudre problème de lock

### Appel fonction 
DMA_jobs(int id) 
DMA_done(int id) -> renvoie son id + signal 1 si traitement bien fait sinon 0 pour erreur

#### Parcours d'une fonction 
1. espace utilisateur appel une fonction DMA
2. va dans le kernel 
3. job qui s'ajoute dans la pile de job
4. DMA qui prend le job 
6. DMA qui load la plage src , write sur trg
7. DMA notifie le kernel pour que le job passe de l'état "en cours" à "fini"





## Hypothèse création structure matérielle du DMA

le DMA est composé en 4 bloc : 
1. Input/ Output -> Snoppy, Maitre et Esclave 
2. architecture du DMA

### Snoppy 
    Vérifie présencce d'un job en attente

### Maitre (output)
    Ordonne load et write <==> envoie un signal vers le disk pour qu'il nous envoie les 

    MAIS COMMENT LE DMA ARRIVE A MODIFIER LES VALEURS D'UNE INFORMATION DANS LE DISQUE ? EN VERILOG ? 

### Esclave (input) 
    signal contenant les tableaux src et trg 

### Architecture 

doit contenir des registre pour les tableaux 
--> mais comment tu définis la taille des tableaux ? 

Signaux entrants : 
src_deb (32 bits)


Question 1 : Comment le DMA arrive à traiter le tableau src ? 





# Partie matérielle 

## Logique matérielle 

Le CPU -> kernel qui ajoute le job dans le liste_job et envoie un signal de notification au DMA qu'il y a un nv job

    - Si le DMA est déjà en train de travailler : 
        cas 1 : cela ne fait rien , au prochain cycle, il oublie l'information 
        cas 2 : il retient l'information et à la fin du job il va chercher le job 
    je pense que le cas 2 est le mieux. 

### Machine d'état (FSM)

1. IDLE : Le DMA attend une notification de nv job
2. FETCH-DESC : Le DMA devient Maitre sur le PiBus.
3. Transfer_DATA
4. Renvoie la data

### Le problème de "Race Condition" (Concurrence)

C'est le point délicat du projet : Que se passe-t-il si le CPU ajoute un job exactement au moment où le DMA finit le dernier et s'apprête à dormir ?



## Composition matérielle 
SNOPPY , MAITRE , ESCLAVE , ARCHITECTURE (CODE)

### ESCLAVE

P_SEL, P_WRITE, P_ADDR, P_DATA_IN, P_DATA_OUT, P_ACK


P_SEL	                Input	Activé quand le processeur accède à l'adresse mémoire du DMA. 
P_WRITE	                Input	'1' pour une écriture dans un registre, '0' pour une lecture.
P_ADDR(1 downto 0)	    Input	Sélectionne quel registre interne est visé (ex: 00=SRC,01=DEST...).
P_DATA_IN(31 downto 0)	Input	La donnée envoyée par le processeur.
P_DATA_OUT(31 downto 0)	Output	La donnée lue par le processeur (ex: pour lire le statut).
P_ACK	                Output	Réponse du DMA pour confirmer la fin de la transaction.

### MAITRE

M_REQ	                Output	Le DMA demande l'accès au bus à l'arbitre.
M_GNT	                Input	L'arbitre accorde le bus au DMA (Grant).
M_ADDR(31 downto 0)	    Output	L'adresse de la RAM que le DMA veut lire ou écrire.
M_READ	                Output	Indique que le DMA veut faire une lecture en RAM.
M_WRITE	                Output	Indique que le DMA veut faire une écriture en RAM.
M_WDATA(31 downto 0)	Output	Donnée que le DMA écrit vers la RAM.
M_RDATA(31 downto 0)	Input	Donnée que le DMA reçoit de la RAM.
M_ACK	                Input	Réponse de la RAM (ou de la cible) confirmant le transfert

## SIGNAUX
CLK, RESET, IRQ



## 1. L'initialisation 
Dès que le module Snoopy, il charge les valeurs de ta structure : 
- src_deb 
- taille

## 2. phase de lecture 
Le DMA ne peut pas "voir" le tableau entier d'un coup. Il procède élément par élément (car le bus ne peut prendre qu'un mot ou octet à la fois )

1. Requete de bus : DMA demande l'autorisation de prendre le contrôle des fils électriques (le bus d'adresses et de données)
2. Emission de l'adresse : DMA palce la valeur de son Registre Adresse Courante sur le bus M_ADDR.
3. active signal lecture de donnée : (le signal traverse le bus vers le disque qui le dit lui renvoie les données src ?)
4. Réception : RAm répond en envoyant la donnée situé à cette adresse.

## 3 logique incrémentation 






# Question : 

Je n'arrive pas à comprendre le fonctionnement du snoopy 

je ne comprends pas la logique du DMA , pour moi, le DMA demande les adr src et trg puis il load le tableau copie vers trg .

Même la je ne comprends pas , comment le DMA arrive a modifier les valeurs des cases/registre trg dans la RAM ou disk ?  
