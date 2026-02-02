# Questions

## S: Je n'arrive pas à comprendre le fonctionnement du snoopy 
A: Le snoopy va surveiller le PiBus pour voir si un nouveau job a été créé. S'il voit qu'un nouveau job est disponible, il notifie le DMA auquel il est associé. Le DMA va ensuite chercher et traiter le job.

## S: Je ne comprends pas la logique du DMA , pour moi, le DMA demande les adr src et trg puis il load le tableau copie vers trg .
A: Le DMA ne demande pas les adr src et trg parce qu'ils sont dans la struct job. Le DMA va récupérer la struct job et il va envoyer des requêtes sur le PiBus. Voici la séquence de requêtes qu'il va sûrement faire:
1. Load case(s) du tableau src
2. Store case(s) récupérées dans le tableau trg
Ensuite il faut faire le diagramme des états selon les différentes réponses du PiBus comme en MULTI

## S:Même la je ne comprends pas , comment le DMA arrive a modifier les valeurs des cases/registre trg dans la RAM ou disk ?
A: Tout d'abord, il n'y a pas de RAM dans notre cas (ou on ne l'utilise pas). Pour modifier les cases du tableau trg, le DMA va envoyer des requêtes sur le PiBus. C'est comme ce que fait le CPU dans le 1er TP de MULTI.  
