CFLAGS = -Wall -g 
CC = gcc

PROGRAMS = sim_dma

.PHONY:	all clean #Commande 'make' rettrouve donc 'all' en premier

all: $(PROGRAMS)

sim_dma: dma.o main.o
	$(CC) -o $@ $(CFLAGS) $^

dma.o: dma.c
	gcc $(CFLAGS) -c dma.c

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean:
	rm -f *.o *~ $(PROGRAMS)
