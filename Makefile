all: server

clean:
	rm -rf *.o
	rm -rf server

server: main.o httpd.o
	gcc -g -o server *.c
main.o: main.c httpd.h
	gcc -g -c -o main.o main.c

httpd.o: httpd.c httpd.h
	gcc -g -c httpd.c  -o httpd.o 

