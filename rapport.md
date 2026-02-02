Ji Stéphane 
Huang Yvonne 
Bakalov Alexander

# Réalisation d'un DMA

Objectif : Création d un composant DMA qui permettera de libérer le CPU des tâches répétitives de transfert de données.

## Hypothèse structure logiciel de DMA 

### Tableau de jobs

Tableau de taille fixe de jobs.
 
Une structure racine contiedra 4 pointeurs qui ponteront vers: 
    - Liste chaînée des cases vides du tableau de jobs
    - Liste chaînée des cases contenant les jobs en attente
    - Liste chaînée des cases contenant les jobs en cours de traitement par le DMA
    - Liste chaînée des cases contenant les job finis

### Structure d'un job

src_deb, src_end, trg_src, trg_end, lock_read, lock_write, taille, id  
logiquement : 8 element * 1 mot = 8 mots, un job a besoin de 8 mots. 

Locks gérés par l'utilisateur.

### Fonctions
DMA_init() -> Appelé lors de la séquence de boot. Place l'adresse de la structure racine dans le DMA et initialise le tableau de jobs.
DMA_job(src, trg, size) -> Appelé par l'utilisateur. src et trg sont des structures contenant les locks et les pointeurs des tableaux src et trg.
DMA_done(int id) -> renvoie son id + signal 1 si traitement bien fait sinon 0 pour erreur.

### Étapes de création d'un job
1. Appel de la fonction DMA_job()
2. syscall (appel l'OS)
3. Lecture de la liste des jobs libres. Si aucune case disponible, on retourne valeur indiquant qu'il n'y a pas la place pour un nouveau job (invitation à appeler DMA_done)
4. Si une case libre est trouvée: vérification du overlap des tableaux. Si il existe un overlap, inverser les adresses src/trg_deb et src/trg_fin
5. Ajout du job dans la liste des jobs en attente
6. Snoopy détecte changement (plus de variable empty, comment faire?) et notifie le(s) DMA(s)

### Étapes de traitement d'un job
1. DMA(s) réveillé(s) par Snoopy. Lecture des jobs en attente et vérifications des locks
2. Traitement du premier job valide trouvé (job passe dans la liste 'en cours de traitement') ou retour à l'état idle si aucun job valide (cela veut dire que le Snoopy doit regarder les locks aussi?)
3. Job traité passe dans la liste des jobs finis)

### Étapes de libération d'un job fini
1. Utilisateur appelle la fonction DMA_done()
2. syscall (appel l'OS)
3. Recherche du job avec le même id que l'argument passé dans toutes les listes en commencant par la liste des jobs finis. Si le job n'est pas trouvé, on renvoie un code d'erreur.
4. Si le job est dans la liste des jobs finis, alors passe le job à la liste des cases vides (sans changer ses champs, ils seront ecrasés par un nouveau job)
5. Sinon:
5.1 Version bloquante: Boucle sur la liste des jobs finis jusqu'à que le job apparaisse dedans.
5.2 Version non bloquante: Valeur de retour indique la situation du job
6. Retourne valeur indiquant que le job est terminé et libéré

## Hypothèse structure matérielle du DMA

Le DMA est un composant synchrone.
Le DMA est composé de 4 blocs modulaires: 
1. Input/ Output -> Snoopy, Maitre et Esclave
2. Interne -> Architecture du DMA

### Snoopy (input)
    Surveille l'état de la liste des jobs en attente. Notifie son DMA quand la liste a un job valide en attente

### Maitre (output)
    Lance les requêtes pour les load et write du tableau src vers le tableau trg. Emetteur et récepteur.

M_REQ	                Output	Le DMA demande l'accès au bus à l'arbitre.
M_GNT	                Input	L'arbitre accorde le bus au DMA (Grant).
M_ADDR	                Output	L'adresse de la RAM que le DMA veut lire ou écrire.
M_READ	                Output	Indique que le DMA veut faire une lecture en RAM.
M_WRITE	                Output	Indique que le DMA veut faire une écriture en RAM.
M_WDATA         	Output	Donnée que le DMA écrit vers la RAM.
M_RDATA	                Input	Donnée que le DMA reçoit de la RAM.
M_ACK	                Input	Réponse de la RAM (ou de la cible) confirmant le transfert

### Esclave (input) 
    Recoit l'adresse de la racine qui sera sauvegardée dans le DMA pour un accès rapide à la liste des jobs en attente.

### SIGNAUX
CLK, RESET

### Architecture 

doit contenir des registre pour les tableaux 
--> comment tenir compte des différents types des tableaux (word, half, byte) ?

Le CPU -> kernel qui ajoute le job dans le liste_job et envoie un signal de notification au DMA qu'il y a un nv job

    - Si le DMA est déjà en train de travailler : 
        cas 1 : cela ne fait rien , au prochain cycle, il oublie l'information 
        cas 2 : il retient l'information et à la fin du job il va chercher le job 

### Machine d'état (FSM)

A compléter
1. IDLE : Le DMA attend une notification de nv job
2. FETCH-DESC : Le DMA devient Maitre sur le PiBus.
3. Transfer_DATA
