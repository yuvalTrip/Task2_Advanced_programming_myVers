.PHONY: all
all: generator primeCounter

generator:  generator.c
	gcc -o randomGenerator generator.c

primeCounter:	primeCounter_basic.c
	gcc -o primeCounter primeCounter.c -pthread

.PHONY: clean
clean:
	-rm randomGenerator primeCounter 2>/dev/null
