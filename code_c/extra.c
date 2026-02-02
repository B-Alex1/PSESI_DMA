#include"extra.h"
#include"dma.h"
#include<stdio.h>

void show_jobs(){
  printf("Jobs status:\n");
  job* j;

  printf("\tFree jobs:");
  j = r.free;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tPending jobs:");
  j = r.pending;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tProcessing jobs:");
  j = r.processing;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");

  printf("\tDone jobs:");
  j = r.done;
  while(j) {
    printf(" [%d]", j->id);
    j = j->next;
  }
  printf("\n");
}
