all: client.c server.c
	gcc -o server server.c
	gcc -o client client.c
	gcc -o example exampleAudio.c
	gcc -o stream stream.c
	
example: exampleAudio.c
	gcc -o example exampleAudio.c

stream: stream.c
	gcc -o stream stream.c

clean:
	rm -f client server example stream *.o
