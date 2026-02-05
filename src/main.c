#include"dma.h"
#include"dma_sim.h"
#include<stdio.h>

#define N 10

int main (int argc, char* argv[]) {
  DMA_init();
  show_jobs();

  // Initialisation des tableaux
  int tab1[N], tab2[N], tab3[N];
  for(int i = 0; i < N; i++) {
    tab1[i] = i;
    tab2[i] = 0;
    tab3[i] = N-i;
    printf("tab1[%d] = %d  |  tab2[%d] = %d |  tab2[%d] = %d\n", i, tab1[i], i, tab2[i], i, tab3[i]);
  }

  uintptr_t job1_id = DMA_job(tab1, tab2, N);
  show_jobs();
  uintptr_t job2_id = DMA_job(tab2, tab3, N);
  show_jobs();

  DMA_sim();
  show_jobs();
  DMA_sim();
  show_jobs();

  DMA_done(job1_id);
  for(int i = 0; i < N; i++) {
    printf("tab1[%d] = %d  |  tab2[%d] = %d |  tab2[%d] = %d\n", i, tab1[i], i, tab2[i], i, tab3[i]);
  }
  show_jobs();

  DMA_done(job2_id);
  for(int i = 0; i < N; i++) {
    printf("tab1[%d] = %d  |  tab2[%d] = %d |  tab2[%d] = %d\n", i, tab1[i], i, tab2[i], i, tab3[i]);
  }
  show_jobs();
  
  return 0;
}
