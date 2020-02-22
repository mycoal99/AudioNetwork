all: client.c server.c
	gcc -o server server.c
	gcc -o client client.c

example: exampleAudio.c
	gcc -o example exampleAudio.c
