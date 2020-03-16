#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#define DR_FLAC_IMPLEMENTATION
#include "./extras/dr_flac.h" /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "./extras/dr_mp3.h" /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "./extras/dr_wav.h" /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include "./miniaudio.h"

int main(int argc, char* argv[]) {

    //check that client did its job correctly
    if (argc != 4) {
        printf("incorrect usage\nCorrect Usage: \'./stream <filename> <pipefd> <IP Address>\'");
    }

    //pipe stuff for talking to client.c other stuff is for connection
    int pipefd = atoi(argv[2]);
    char done[2];
    int portOffset = 0;
    int sockfd = 0, bytesReceived = 0;
    char recvBuff[500];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;

    //check socket can be created
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Could not create socket\n");
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8084); // port
    serv_addr.sin_addr.s_addr = inet_addr(argv[3]);
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // open temp file that will serve as buffer.
    FILE *fp;
    fp = fopen("test.mp3", "w"); 
    if(NULL == fp){
        printf("Unable to open file");
        return 1;
    }

    char* timeBuff = calloc(1,8);
    bytesReceived = read(sockfd, timeBuff, sizeof(timeBuff));
    write(pipefd, timeBuff, sizeof(timeBuff));

    //recieve bytes from server and write to temp file
    while(1) {
        // printf("Bytes received %c\n",bytesReceived);
        bytesReceived = read(sockfd, recvBuff, 500);   
        fwrite(recvBuff, 1,bytesReceived,fp);
        // if (bytesReceived < 500)
        //     break;
    }

    // Read close signal from client and send to server (ignore for now)

    close(sockfd);

    return 0;
}