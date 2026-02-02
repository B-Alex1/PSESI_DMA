#include"dma_sim.h"
#include<stdio.h>

/*
 * Simulation du comportement de la DMA
 */
void DMA_prend() {
  job* j = r.pending;
 
  if(j) {
    r.pending = j->next;
    if(r.pending) {
      (r.pending)->prec = NULL;
    }

    j->next = r.processing;
    r.processing = j;
    (j->next)->prec = j;

    DMA_sim(j->src, j->trg, j->size, j->id);
  }

  // Un autre DMA a peut-être pris le job
}

/*
 * Simulation du travail de la DMA
 */
void DMA_sim(int* src, int* trg, size_t size, int id) {
  for(int i = 0; i < size; i++) {
    trg[i] = src[i];
  }

  DMA_fini(id);
}

/*
 * Appelé par le DMA après avoir traité un job
 */
void DMA_fini(int id) {
  job* j = r.processing;
  while(j) {
    if(j->id == id) {
      (j->prec)->next = j->next;
      (j->next)->prec = j->prec;

      j->next = r.done;
      j->prec = NULL;
      r.done = j;
    }
  }

  fprintf(stderr, "Processed job not found\n");
}
