.PHONY: all
all: generator primeCounter queue primeCounter_basic

generator:  generator.c
	gcc -g -o randomGenerator generator.c

primeCounter:	primeCounter.c queue.o
	gcc -g -o primeCounter primeCounter.c queue.o -pthread

primeCounter_basic:	primeCounter_basic.c
	gcc -g -o primeCounter_basic primeCounter_basic.c

queue:	queue.c queue.h
	gcc -c -g -o queue.o queue.c
.PHONY: clean

clean:
	-rm randomGenerator primeCounter queue.o 2>/dev/null
