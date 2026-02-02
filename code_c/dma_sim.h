#ifndef DMA_SIM
#define DMA_SIM

#include"dma.h"

void DMA_prend();
void DMA_sim(int* src, int* trg, size_t size, int id);
void DMA_fini(int id);

#endif
