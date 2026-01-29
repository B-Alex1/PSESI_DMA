Ji Stéphane 
Huang Yvonne 
Bakalov Alexander

# Réalisation DMA pour l'IA 

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