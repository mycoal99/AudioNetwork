#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    int listenfd[10];
    int connfd[10] = {0};
    struct sockaddr_in serv_addr[10];
    char sendBuff[10][1024];
    int numrv;
    int connected[10];
    int numConnections = 0;
    while(numConnections<10) {
        
        listenfd[numConnections] = socket(AF_INET, SOCK_STREAM, 0);

        fprintf(stderr, "%d\n", listenfd[numConnections]);

        memset(&serv_addr[numConnections], '0', sizeof(serv_addr[numConnections]));
        memset(sendBuff[numConnections], '0', sizeof(sendBuff[numConnections]));

        serv_addr[numConnections].sin_family = AF_INET;
        serv_addr[numConnections].sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr[numConnections].sin_port = htons(8081+numConnections);
        fprintf(stderr, "%d\n", ntohs(serv_addr[numConnections].sin_port));


        bind(listenfd[numConnections], (struct sockaddr*)&serv_addr[numConnections],sizeof(serv_addr[numConnections]));
        setsockopt(listenfd[numConnections], SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
        listen(listenfd[numConnections], 10);
        connfd[numConnections] = accept(listenfd[numConnections], (struct sockaddr*)NULL ,NULL);
        // if (connected[numConnections]) {
            
        // }
        connected[numConnections] = 1;
        


        fprintf(stderr, "%d\n", numConnections);

        int PID = fork();
        numConnections++;
        // fprintf(stderr, "%d\n", numConnections);
        if (!PID) {
            while(1){
                
                FILE *fp; 
                fp = fopen("pink_panther.mp3","rb");

                while(1){
                    unsigned char buff[500]={0};
                    char done[1] = {0};
                    int readCount = fread(buff,1,500,fp);
                    // printf("Bytes read %d \n", readCount);        

                    if(readCount > 0){
                        // printf("Sending \n");
                        write(connfd[numConnections-1], buff, readCount);
                    }
                    else{
                        // printf("Probably done\n");
                        read(connfd[numConnections-1], done, 1);
                        printf("%s\n", done);
                        if (!strcmp(done, "q")) {
                            close(connfd[numConnections-1]);
                            sleep(1);
                            return 0;
                        } else continue;
                    }
                }
                close(connfd[numConnections-1]);
                sleep(1);
                return 0;
            }
        } 
        // else {
        //     numConnections++;
            
        //     continue;
        // }
    }
    if (numConnections == 10)
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