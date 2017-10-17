all: hw5 test
hw5: hw5.o
	gcc -o hw5 hw5.o
hw5.o: hw5.c
	gcc -c hw5.c
test: test.o
	gcc -o test test.o
test.o: test.c
	gcc -c test.c
dep:
	gcc -M *.c >.depend
clean: 
	 rm -f *.o hw5 test
