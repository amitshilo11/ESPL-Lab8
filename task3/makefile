all: exec

c-files: fibb.o task3.o patch.o
	gcc -g -c -o fibb.o fibb.c
	gcc -g -c -o task3.o task3.c
	gcc  -g -c -o patch.o patch.c

exec: c-files fibb.o task3.o patch.o
	gcc -g fibb.o -o fibonacci
	gcc -g task3.o -o task3
	gcc  -g patch.o -o patch
	rm -f fibb.o task3.o patch.o
	
.PHONY: clean
clean:
	rm -rf ./*.o fibonacci
