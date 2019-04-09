all: main.c
	gcc -g -Wall -o imageflip main.c

clean:
	rm imageflip
