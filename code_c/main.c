#include"dma.h"
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

  tab_lock* tl_tab1 = make_tab_lock(tab1, SRC_LOCK);
  tab_lock* tl_tab2 = make_tab_lock(tab2, TRG_LOCK);
    
  int job = DMA_job(tl_tab1, tl_tab2, N);
  show_jobs();

  DMA_sim();
  show_jobs();

  DMA_done(job);
  for(int i = 0; i < N; i++) {
    printf("tab1[%d] = %d  |  tab2[%d] = %d\n", i, tab1[i], i, tab2[i]);
  }
  show_jobs();

  free_tab_lock(tl_tab1);
  free_tab_lock(tl_tab2);

  return 0;

}
