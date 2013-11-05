all: serial paralel
serial:
	gcc serial.c -o serial
paralel:
	gcc -fopenmp paralel.c -o paralel
clean:
	rm -rf *o
