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
  job* j = &(jobs[0]);

  r.free = j;
  j->id = (int)(&(jobs[0]));
  j->prec = NULL;
  for(int i = 1; i < MAX_JOBS; i++) {
    j->next = &(jobs[i]);
    j = j->next;
    j->prec = &(jobs[i-1]);
    j->id = (int)(&(jobs[i]));
  }
  j->next = NULL;
}

/* 
 * Ajout d'un job
 * Exécuté par l'OS après le syscall de DMA_job
 * A FAIRE: savoir gérer différents types de tableau
 */
int DMA_job(int* src, int* trg, size_t size) {
  job* j = r.free;
  if (!j) {
    printf("ERROR: Job list is full!\n");
    return -1;
  }

  r.free = j->next;
  if(r.free) {
    (r.free)->prec = NULL;
  }
  
  j->src = src;
  j->trg = trg;

  j->size = size;

  j->next = r.pending;
  j->prec = NULL;
  r.pending = j;

  return j->id;
}

/* 
 * Vérification de la fin d'un job et libération de ce job
 * Exécuté par l'OS après le syscall de DMA_done
 * Version bloquante
 */
int DMA_done(int id) {
  job *j;
  j = r.done;

  while(j) {
    if (j->id == id) {
      // On a trouvé le job!
      if(j->prec) {
        (j->prec)->next = j->next;
        (j->next)->prec = j->prec;
      } else {
        r.done = j->next;
        (j->next)->prec = NULL;
      }

      j->next = r.free;
      j->prec = NULL;
      r.free = j;

      return 1;
    }
  }

  fprintf(stderr, "Job not found in DMA_done\n");
  return -1;
}
