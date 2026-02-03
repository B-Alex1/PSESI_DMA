# PSESI:
 objectif: conception structure de donnée pour la gestion d'une DMA

 ### Contexte:
 Dans le fonctionnement d'un processeur nous venons parfois a faire des copie en mémoire d'un adressse source a une adresse destination, nous copions octet par octet. c'est une tache qui est fastidieuse et qui ne necessite pas d'intélligense. surtout dans le contexte d'évolution de l'ia avec les calcules par matrice.
 Il existe alors un micro-controler la DMA, a qui le processeur délègue cette tache mécanique. Pour cela on doit avoir une strcucture qui permet de communiquer les taches donné a la dma par le processeur et transmettre les informations necessaires à la dma
 - établir la communication entre DMA et processeur
 - une API ?

 ### solution
Pour résoudre ce problème nous avons implémenté une structure en mémoire qui permet de donner les paramètre necessaire au DMA:

Nous commençons par une structure job:

    job:
    -s // l'adresse source
    -t //l'addresse destination
    -size //la taille de la donnée à transformé


Elle sera manipulé par :
    -La DMA : reçois et traite la demande
    -Le Processeur : crée le job et le récupère le travaille terminé

Nous avons une structure global qui stocke les travaux et qui permet a la DMA et au processeur de communiquer. Un job peut avoir plusieur état:
    -pending: en attente des d'être traité
    -processing: en cours de traitement par la DMA
    -done: traité et en attente d'être acquitté par le processeur

Nous avons une structure racine en mémoire qui est composé de plusieur pointeurs, nous avons un pointeur par état, chaque pointeur pointe sur une liste de job:
    - pending //liste de job en attente d'etre traité
    - processing //liste de job en cours de traitement par la DMA
    - done //liste de job fini attendant d'être acquité par le processeur
    - free //liste de structure job libre en mémoire

Les donnée étant manipulé sous forme de liste, on va ajouté 2 pointers dans la structure de donnée job:
    -next //pointeur vers le job suivant, Null si le dernier de la liste
    -prec //pointeur vers le job précèdent, Null si premier

Un tableau de job étant alloué en mémoire pour stcoker un nombre de job maximal en mémoire.

Fonction a implémenté coté processeur:

*DMA_submit: insérer un nouveau job dans la structure, assurer la validité de la structure en mémoire
*DMA_done: appelé pour vérifier et acquité de la tache, re rendre la structure de job libre(libère la resource)

Fonction implémenté pour simuler le comportement d'une DMA:

*DMA_proces: prend en charge un job attendant d'etre traité, traite
*DMA_fini: rend un job dans un état fini, qui attend d'être acquittté par le processeur

### Problème de concurrence
La structure étant manipulé par la DMA et le processeur, elle est soumise a des data races,dans la structure racine nous veillerons a aire un vérrouillage fin, chaque pointeur représentant un état de la liste sera protégé par un verrou lorsqu'elle sera manipulé.

### Pseudo code des fonction vu processeur

#### DMA_submit:
paramètre( source, target, size, id) //id sera addresse de la case du job ?? non on l'a pas encore 
    lock (racine->free){
        job=racine.free
        racine.free++
        si(racine.free){
            
        }
    }

//dsl je crois que je sais pas écrire de pseudo code, on dirait que jevais l'écrire en c la mdrrr