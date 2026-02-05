CFLAGS = -Wall -g 
CC = gcc

PROGRAMS = sim_dma

.PHONY:	all clean #Commande 'make' rettrouve donc 'all' en premier

all: $(PROGRAMS)

sim_dma: dma.o dma_sim.o main.o
	$(CC) -o $@ $(CFLAGS) $^

%.o: %.c
	gcc $(CFLAGS) -c $^

clean:
	rm -f *.o *~ $(PROGRAMS)
