all: hw4

hw4: mm.o hw.o
	gcc -Wall -g -o hw4  mm.o hw.o

hw.o: hw.c
	gcc -Wall -g -c hw.c

mm.o: mm.c
	gcc -Wall -g -c mm.c

dep: 
	gcc -H  mm.c hw.c >.depend

clean:
	rm -f hw *.o hw4 .depend
