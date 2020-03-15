all: client.c server.c
	gcc -o server server.c -ldl -lpthread -lm
	gcc -o client client.c -ldl -lpthread -lm
	gcc -o example exampleAudio.c -ldl -lpthread -lm
	gcc -o stream stream.c -ldl -lpthread -lm
	
example: exampleAudio.c
	gcc -o example exampleAudio.c

stream: stream.c
	gcc -o stream stream.c

clean:
	rm -f test.mp3 client server example stream *.o
