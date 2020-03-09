#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

int main(void)
{
    int listenfd = 0;
    int connfd = 0;
    int written = 0;

    struct sockaddr_in serv_addr;
    struct timeval time;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    listen(listenfd, 10);


    while(1){
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
        FILE *fp; 
        fp = fopen("pink_panther.mp3","r");

        //TODO: Calculate start time that starts playback on all slave devices.


        gettimeofday(&time,NULL);
        long int ms = ((time.tv_sec * 1000) + (time.tv_usec / 1000) + 4000)%10000; //Modulo used to simplify the timestamp
        char* timeBuff = calloc(1,8);

        // Convert LI to char*
        printf("%lu\n",sizeof(timeBuff[0]));
        sprintf(timeBuff,"%li",ms);

        fprintf(stderr, "Sending start time stamp: %li\n",ms);  

        int timeSize = sizeof(timeBuff[0]);
        int written = 0;

        //Write entire time packet
        while(timeSize > 0){
            written = write(connfd,timeBuff,sizeof(timeBuff));
            timeSize -= written; 
        } 

        //Write audiofile
        while(1){
            unsigned char sendBuff[500]= {0};
            int readCount = fread(sendBuff,1,500,fp); 
            fprintf(stderr, "Bytes read %d \n", readCount);
            if(readCount > 0){
                fprintf(stderr, "Sending \n");
                write(connfd, sendBuff, readCount);
            }
            else{
                fprintf(stderr, "Probably done\n");
                break;
            }
        }
        close(connfd);
        sleep(1);
    }

    return 0;
}