#include"dma_sim.h"
#include<stdio.h>
#include<pthread.h>

/*
 * Simulation du comportement de la DMA
 */
job* DMA_prend() {
  job* j = r.pending;
 
  if(j) {
    if(!remove_job(j, &(r.pending))) {
      fprintf(stderr, "ERROR: could not remove job from pending list\n");
      return NULL;
    }
    
    if(!add_job(j, &(r.processing))) {
      fprintf(stderr, "ERROR: could not add job to processing list\n");
      return NULL;
    }

    return j;
  }

  // Un autre DMA a peut-être pris le job
  return NULL;
}

/*
 * Simulation du travail de la DMA
 */
void DMA_sim() {
  job* j;
//  while(true) {
    j = DMA_prend();
    if(j) {
      for(int i = 0; i < j->size; i++) {
        j->trg[i] = j->src[i];
      }
  
      if(!DMA_fini(j)) {
        fprintf(stderr, "ERROR: could not finish job\n");
      }
    }
    // TODO Dormir?
//  }
}

/*
 * Appelé par le DMA après avoir traité un job
 */
int DMA_fini(job* j) {
  if(!remove_job(j, &(r.processing))) {
    fprintf(stderr, "ERROR: could not remove job from processing list\n");
    return 0;
  }
  
  if(!add_job(j, &(r.done))) {
    fprintf(stderr, "ERROR: could not add job to done list\n");
    return 0;
  }

  return 1;
}
