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
  j->id = 0;
  for(int i = 1; i < MAX_JOBS; i++) {
    j->next = &(jobs[i]);
    j = j->next;
    j->id = i;
  }
  j->next = NULL;
}

/* 
 * Ajout d'un job
 * Exécuté par l'OS après le syscall de DMA_job
 */
int DMA_job(tab_lock* src, tab_lock* trg, size_t size) {
  job* j = r.free;
  if (!j) {
    printf("ERROR: Job list is full!\n");
    return -1;
  }

  r.free = j->next;
  
  // A FAIRE: vérification du overlap
  j->src_start = src->tab;
  j->src_end = (src->tab)+size;
  j->trg_start = trg->tab;
  j->trg_end = (trg->tab)+size;

  j->src_lock_read = src->lock_read;
  j->src_lock_write = src->lock_write;
  j->trg_lock_read = trg->lock_read;
  j->trg_lock_write = trg->lock_write;

  j->size = size;

  // Ajouter le nouveau job en tete de pending
  j->next = r.pending;
  r.pending = j;

  return j->id;
}

/*
 * Simulation du comportement de la DMA
 * Seulement utilisée pour le code C, n'existe pas ailleurs dans le projet
 * Encore incomplète
 */
void DMA_sim() {
  job* j = r.pending;
  if(j->src_lock_read || j->trg_lock_write) {
    printf("Cannot perform job [%d] because of locks\n", j->id);
    return;
  }
 
  // Job passe de pending à processing 
  r.pending = j->next;
  j->next = r.processing;
  r.processing = j;

  show_jobs(); // Montrer que le job est passé en processing
  
  for(int i = 0; i < j->size; i++) {
    j->trg_start[i] = j->src_start[i];
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
      if (j->id == id) {
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

tab_lock* make_tab_lock(int* tab, int flag) {
  tab_lock* res = malloc(sizeof(tab_lock));
  res->tab = tab;
  switch(flag) {
    case NO_LOCK:
      res->lock_read = 0;
      res->lock_write = 0;
      break;
    case SRC_LOCK:
      res->lock_read = 0;
      res->lock_write = 1;
      break;
    case TRG_LOCK:
      res->lock_read = 1;
      res->lock_write = 0;
      break;
    default:
      printf("Unrecognized flag in make_tab_lock\n");
      return NULL;
  }
  return res;
}

void reset_tab_lock(tab_lock* tl) {
  tl->lock_read = 0;
  tl->lock_write = 0;
}

void free_tab_lock(tab_lock* tl) {
  free(tl);
}

void show_jobs(){
  printf("Jobs status:\n");
  job* j;

  printf("\tFree jobs:");
  j = r.free;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tPending jobs:");
  j = r.pending;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tProcessing jobs:");
  j = r.processing;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tDone jobs:");
  j = r.done;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");
}
