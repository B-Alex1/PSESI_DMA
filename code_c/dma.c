#include"dma.h"
#include<stdio.h>

job jobs[MAX_JOBS];
racine r;

/* 
 * Initialisation de la DMA
 * Appelé lors du boot
 */
void DMA_init() {
  
  r.pending = NULL;
  r.processing = NULL;
  r.done = NULL;
  
  // Toutes les cases du tableau de jobs sont vides au début
  r.free = &(jobs[0]);
  job* j = r.free;
  j->num = 0;
  for(int i = 1; i < MAX_JOBS; i++) {
    j->next = &(jobs[i]);
    j = j->next;
    j->num = i;
  }
  j->next = NULL;
}

/* 
 * Ajout d'un job
 * Exécuté par l'OS après le syscall de DMA_job
 */
int DMA_job(tab_lock src, tab_lock trg, size_t size) {
  job* j = r.free;
  if (!j) {
    printf("ERROR: Job list is full!\n");
    return -1;
  }

  r.free = j->next;
  
  j->src = src;
  j->trg = trg;
  j->size = size;


  // Ajouter le nouveau job en tete de pending
  j->next = r.pending;
  r.pending = j;

  return j->num;
}

/*
 * Simulation du comportement de la DMA
 * Seulement utilisée pour le code C, n'existe pas ailleurs dans le projet
 * Encore incomplète
 */
void DMA_sim() {
  job* j = r.pending;
  if(j->src.lock_src || j->trg.lock_trg) {
    printf("Cannot perform job [%d] because of locks\n", j->num);
    return;
  }
 
  // Job passe de pending à processing 
  r.pending = j->next;
  j->next = r.processing;
  r.processing = j;

  show_jobs(); // Montrer que le job est passé en processing
  
  for(int i = 0; i < j->size; i++) {
    j->trg.tab[i] = j->src.tab[i];
  }

  // Job passe de processing à done
  r.processing = j->next;
  j->next = r.done;
  r.done = j;
}

/* 
 * Vérification de la fin d'un job
 * Exécuté par l'OS après le syscall de DMA_done
 * Version bloquante
 */
int DMA_done(int id) {
  job *j, *prec;
  int timeout = TIMEOUT;

  // Bloquant pendant TIMEOUT itérations
  while(timeout) {
    j = r.done;
    prec = NULL;

    while(j) {
      if (j->num == id) {
        // On a trouvé le job!
        if(prec) {
          prec->next = j->next;
        } else {
          r.done = j->next;
        }
        j->next = r.free;
        r.free = j;

        return 1;
      }
      
      prec = j;
      j = j->next;
    }
    timeout--;
  }

  printf("ERROR: Timeout waiting for job %d to finish\n", id);
  return -1;
}

void show_jobs(){
  printf("Jobs status:\n");
  job* j;

  printf("\tFree jobs:");
  j = r.free;
  while(j) {
    printf(" [%d]", j->num);
    j = j->next;
  }
  printf("\n");

  printf("\tPending jobs:");
  j = r.pending;
  while(j) {
    printf(" [%d]", j->num);
    j = j->next;
  }
  printf("\n");

  printf("\tProcessing jobs:");
  j = r.processing;
  while(j) {
    printf(" [%d]", j->num);
    j = j->next;
  }
  printf("\n");

  printf("\tDone jobs:");
  j = r.done;
  while(j) {
    printf(" [%d]", j->num);
    j = j->next;
  }
  printf("\n");
}
