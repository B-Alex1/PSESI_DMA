#ifndef DMA_H
#define DMA_H

#include<stdint.h>
#include<stddef.h>
#include<stdlib.h>

#define MAX_JOBS 10

typedef struct s_job {
  int* src;
  int* trg;
  size_t size;
  int id;
  struct s_job *prec;
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
int DMA_job(int* src, int* trg, size_t size);
int DMA_done(int id);

// Fonctions utiles pour le main

#endif // _DMA_H
