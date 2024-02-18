source = main.c shell.c
A = $(source:.c=.o)

all:dep SHELL clean

SHELL:$(A)
	gcc -o $@ $(A)

dep:
	gcc -MM $(source) > dep

clean:
	rm -f *.o
