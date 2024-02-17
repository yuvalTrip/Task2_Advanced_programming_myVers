.PHONY: all
all: generator primeCounter queue primeCounter_basic

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter.c queue.o
	gcc -o primeCounter primeCounter.c queue.o -pthread

primeCounter_basic:	primeCounter_basic.c
	gcc -o primeCounter_basic primeCounter_basic.c

queue:	queue.c queue.h
	gcc -c -o queue.o queue.c
.PHONY: clean

clean:
	-rm randomGenerator primeCounter queue.o 2>/dev/null
