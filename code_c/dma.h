#ifndef DMA_H
#define DMA_H

#include<stdint.h>
#include<stddef.h>

#define MAX_JOBS 10
#define TIMEOUT 10

typedef struct s_tab_lock {
  uint8_t lock_src;
  uint8_t lock_trg;
  int* tab;
} tab_lock;

typedef struct s_job {
  tab_lock src;
  tab_lock trg;
  size_t size;
  uint8_t num;
  struct s_job *next;
} job;

extern job jobs[MAX_JOBS];

typedef struct s_racine {
  job* pending;
  job* free;
  job* processing;
  job* done;
} racine;

extern racine r;

void DMA_init();
int DMA_job(tab_lock src, tab_lock trg, size_t size);
void DMA_sim(); // Simule comportement de la DMA
int DMA_done(int id);

void show_jobs();

#endif // _DMA_H
