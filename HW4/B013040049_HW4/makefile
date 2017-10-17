all: B013040049_HW4

B013040049_HW4: mm.o B013040049_HW4.o
	gcc -Wall -g -o B013040049_HW4 mm.o B013040049_HW4.o

B013040049_HW4.o: B013040049_HW4.c
	gcc -Wall -g -c B013040049_HW4.c

mm.o: mm.c mm.h
	gcc -Wall -g -c mm.c

dep: 
	gcc -H  mm.c B013040049_HW4.c >.depend

clean:
	rm -f *.o B013040049_HW4 .depend
