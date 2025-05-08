all: server

clean:
	rm -rf *.o
	rm -rf server

server: main.o httpd.o
	gcc -g -O0 -fno-stack-protector -no-pie -o server *.c
main.o: main.c httpd.h
	gcc -g -O0 -fno-stack-protector -no-pie -c -o main.o main.c

httpd.o: httpd.c httpd.h
	gcc -g -O0 -fno-stack-protector -no-pie -c httpd.c  -o httpd.o 

