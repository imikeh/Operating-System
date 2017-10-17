all: B013040049_Shell

B013040049_Shell: B013040049_Shell.o
	gcc -o B013040049_Shell B013040049_Shell.o

B013040049_Shell.o:
	gcc -c B013040049_Shell.c

dep:
	gcc -M *.c > .depend

clean:
	rm -f *.o B013040049_Shell .depend
