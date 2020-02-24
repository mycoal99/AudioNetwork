all: client.c server.c
	gcc -o server server.c -lpthread -lm -ldl
	gcc -o client client.c -lpthread -lm -ldl
	gcc -o example exampleAudio.c -lpthread -lm -ldl

example: exampleAudio.c
	gcc -o example exampleAudio.c -lpthread -lm -ldl

clean:
	rm -rf server client example *.o
