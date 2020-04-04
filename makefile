
build:
	gcc main.c -o main.o

run:
	gcc main.c -o main.o
	./main.o

default:
	run