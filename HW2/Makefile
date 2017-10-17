all: B013040049_HW2

B013040049_HW2: B013040049_HW2.o
	gcc -o B013040049_HW2 B013040049_HW2.o -pthread

B013040049_HW2.o:
	gcc -c B013040049_HW2.c

dep:
	gcc -M *.c > .depend

clean:
	rm -f *.o B013040049_HW2 .dependHW2
