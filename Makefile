all:
	gcc main.c ./src/tls.c -Xlinker --wrap=pthread_create -lpthread -o test -I./src/include