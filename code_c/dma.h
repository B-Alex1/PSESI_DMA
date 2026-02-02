#ifndef DMA_H
#define DMA_H

#include<stdint.h>
#include<stddef.h>
#include<stdlib.h>

#define MAX_JOBS 10
#define TIMEOUT 10

#define NO_LOCK 0
#define SRC_LOCK 1
#define TRG_LOCK 2

typedef struct s_tab_lock {
  uint8_t lock_read;
  uint8_t lock_write;
  int* tab;
} tab_lock;

typedef struct s_job {
  int* src_start;
  int* src_end;
  int* trg_start;
  int* trg_end;
  uint8_t src_lock_read;
  uint8_t src_lock_write;
  uint8_t trg_lock_read;
  uint8_t trg_lock_write;
  size_t size;
  uint8_t id;
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
int DMA_job(tab_lock* src, tab_lock* trg, size_t size);
void DMA_sim(); // Simule comportement de la DMA
int DMA_done(int id);

// Fonctions utiles pour le main
tab_lock* make_tab_lock(int* tab, int flag);
void reset_tab_lock(tab_lock* tl);
void free_tab_lock(tab_lock* tl);
void show_jobs();

#endif // _DMA_H
