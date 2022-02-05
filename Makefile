
all: life

life: life.c
	gcc -std=c99 -o life life.c

clean:
	rm -f life