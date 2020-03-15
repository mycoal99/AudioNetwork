#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    int listenfd;
    int connfd[10];
    struct sockaddr_in serv_addr;
    // printf("hello There\n");
    char sendBuff[1024];
    int numrv;
    int numConnections = 0;
        

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    int iSetOption = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8084);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    listen(listenfd, 10);

    while(1) {
        connfd[numConnections] = accept(listenfd, (struct sockaddr*)NULL ,NULL);

        int PID = fork();
        int childConnfd = connfd[numConnections];
        int numChildfd = numConnections;
        if (PID == 0) {
            while(1){
                
                FILE *fp; 
                fp = fopen("00602567992424_007_100 Miles and Running_USUM71815294.mp3","rb");

                while(1){
                    // char buffFinished[4];
                    // snprintf(buffFinished, 4, "xyz");
                    unsigned char buff[500]={0};
                    char done[2];
                    int readCount = fread(buff,1,500,fp);
                    // printf("Bytes read %d \n", readCount);        

                    if(readCount > 0){
                        // printf("Sending \n");
                        write(childConnfd, buff, readCount);
                        continue;
                    }
                    else{
                        fprintf(stderr, "made it to read!\n");
                        numConnections--;
                        close(childConnfd);
                        sleep(1);
                        return 0;
                        // // fprintf(stderr, "%s\n", buffFinished);
                        // // fprintf(stderr, "reached write q, size: %d\n", sizeof(buffFinished));
                        // // write(childConnfd, buffFinished, sizeof(buffFinished));
                        // fprintf(stderr, "made it to read!\n");
                        // // fprintf(stderr, "%d\n", numConnections);
                        // read(childConnfd, done, sizeof(done));
                        // fprintf(stderr,"Passed Read\n");
                        // if (!strcmp(done, "q")) {
                        //     fprintf(stderr,"Connection %d closing.\n", numChildfd);
                        //     close(childConnfd);
                        //     sleep(1);
                        //     return 0;
                        // }
                        // else {
                        //     continue;
                        // }
                        // break;
                    }
                }
                numConnections--;
                close(childConnfd);
                sleep(1);
                return 0;
            }
        }
        else {
            fprintf(stderr, "listening thread incrementing\n");
            numConnections++;
            continue;
        }

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