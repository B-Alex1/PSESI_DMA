#include"dma.h"
#include<stdio.h>

job jobs[MAX_JOBS];
racine r;

/*
 * Verrouille la liste et retirie job de la liste
 * Retourne 1 si réussi, 0 sinon
 */
int remove_job(job* j, job **list) {
  if(!j || !list) {
    return 0;
  }

  // TODO Verrouiller liste

  if(*list == j) {
    *list = j->next;
  } else {
    (j->prec)->next = j->next;
  }

  if(j->next) {
    (j->next)->prec = j->prec;
  }

  // TODO Deverrouiller liste
  return 1;
}

/*
 * Verrouille la liste et ajoute le job en tete de la liste
 * Retourne 1 si réussi, 0 sinon
 */
int add_job(job* j, job **list) {
  if(!j || !list) {
    return 0;
  }

  // TODO Verrouiller liste

  if(*list) {
    (*list)->prec = j;
  }
  j->next = *list;
  *list = j;

  //TODO Verrouiller liste
  return 1;
}


/* 
 * Initialisation de la DMA
 * Appelé lors du boot
 */
void DMA_init() {
  
  r.pending = NULL;
  r.processing = NULL;
  r.done = NULL;
  
  // Toutes les cases du tableau de jobs sont vides au début
  job* j = &jobs[0];

  r.free = j;
  j->id = (uintptr_t)&j;
  j->prec = NULL;
  for(int i = 1; i < MAX_JOBS; i++) {
    j->next = &(jobs[i]);
    j = j->next;
    j->prec = &jobs[i-1];
    j->id = (uintptr_t)&j;
  }
  j->next = NULL;

  // TODO Lancer le thread du DMA
}

/* 
 * Ajout d'un job
 * Exécuté par l'OS après le syscall de DMA_job
 */
uintptr_t DMA_job(int* src, int* trg, size_t size) {
  job* j = r.free;
  if (!j) {
    printf("ERROR: Job list is full!\n");
    return 0;
  }

  if(!remove_job(j, &(r.free))) {
    fprintf(stderr, "ERROR: could not remove job from free list\n");
    return 0;
  }

  j->src = src;
  j->trg = trg;
  j->size = size;

  if(!add_job(j, &(r.pending))) {
    fprintf(stderr, "ERROR: could not add job to pending list\n");
  }

  return j->id;
}

/* 
 * Vérification de la fin d'un job et libération de ce job
 * Exécuté par l'OS après le syscall de DMA_done
 * Version bloquante
 */
uintptr_t DMA_done(uintptr_t id) {
  job *j;
  j = r.done;

  while(j) {
    if (j->id == id) {
      if(!remove_job(j, &(r.done))) {
        fprintf(stderr, "ERROR: could not remove job from done list\n");
        return 0;
      }
      
      if(!add_job(j, &(r.free))) {
        fprintf(stderr, "ERROR: could not add job to free list\n");
        return 0;
      }

      return j->id;
    }

    j = j->next;
  }

  fprintf(stderr, "WARNING: Job not found in DMA_done\n");
  return 0;
}

/*
 * Affiche l'état des 4 listes de jobs sur le terminal
 */
void show_jobs(){
  printf("Jobs status:\n");
  job* j;

  printf("\tFree jobs:");
  j = r.free;
  while(j) {
    printf(" [%ld]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tPending jobs:");
  j = r.pending;
  while(j) {
    printf(" [%ld]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tProcessing jobs:");
  j = r.processing;
  while(j) {
    printf(" [%ld]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tDone jobs:");
  j = r.done;
  while(j) {
    printf(" [%ld]", j->id);
    j = j->next;
  }
  printf("\n");
}
