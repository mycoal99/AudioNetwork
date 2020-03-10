#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    int listenfd[100];
    int connfd[100];
    struct sockaddr_in serv_addr[100];
    printf("hello There\n");
    char sendBuff[100][1024];
    int numrv;
    int numConnections = 0;
    while(numConnections<100) {
        

        listenfd[numConnections] = socket(AF_INET, SOCK_STREAM, 0);
        printf("General Kenobi\n");

        memset(&serv_addr[numConnections], '0', sizeof(serv_addr[numConnections]));
        memset(sendBuff[numConnections], '0', sizeof(sendBuff[numConnections]));

        serv_addr[numConnections].sin_family = AF_INET;
        serv_addr[numConnections].sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr[numConnections].sin_port = htons(8080);

        bind(listenfd[numConnections], (struct sockaddr*)&serv_addr[numConnections],sizeof(serv_addr[numConnections]));
        listen(listenfd[numConnections], 10);

        if (!fork()) {
            while(1){
                connfd[numConnections] = accept(listenfd[numConnections], (struct sockaddr*)NULL ,NULL);

                FILE *fp; 
                fp = fopen("pink_panther.mp3","rb");

                while(1){
                    unsigned char buff[500]={0};
                    int readCount = fread(buff,1,500,fp);
                    printf("Bytes read %d \n", readCount);        

                    if(readCount > 0){
                        printf("Sending \n");
                        write(connfd[numConnections], buff, readCount);
                    }
                    else{
                        printf("Probably done\n");
                        return 1;
                    }
                }
                close(connfd[numConnections]);
                sleep(1);
            }
        } else numConnections++;

    }
    if (numConnections == 100)
        sleep(60);


    // while(1){
    //     connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);

    //     FILE *fp; 
    //     fp = fopen("pink_panther.mp3","rb");

    //     while(1){
    //         unsigned char buff[500]={0};
    //         int readCount = fread(buff,1,500,fp);
    //         printf("Bytes read %d \n", readCount);        

    //         if(readCount > 0){
    //             printf("Sending \n");
    //             write(connfd, buff, readCount);
    //         }
    //         else{
    //             printf("Probably done\n");
    //             return 1;
    //         }
    //     }
    //     close(connfd);
    //     sleep(1);
    // }


    return 0;
}