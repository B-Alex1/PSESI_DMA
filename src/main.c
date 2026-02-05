#include"dma.h"
#include"dma_sim.h"
#include<stdio.h>

#define N 10

int main (int argc, char* argv[]) {
  DMA_init();

  show_jobs();

  int tab1[N];
  int tab2[N];
  for(int i = 0; i < N; i++) {
    tab1[i] = i;
    tab2[i] = 0;
    printf("tab1[%d] = %d  |  tab2[%d] = %d\n", i, tab1[i], i, tab2[i]);
  }

  uintptr_t job_id = DMA_job(tab1, tab2, N);
  show_jobs();

  DMA_sim();
  show_jobs();

  DMA_done(job_id);
  for(int i = 0; i < N; i++) {
    printf("tab1[%d] = %d  |  tab2[%d] = %d\n", i, tab1[i], i, tab2[i]);
  }
  show_jobs();
  
  return 0;
}
