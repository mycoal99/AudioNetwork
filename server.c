#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1024];
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    listen(listenfd, 10);


    while(1){
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);

        FILE *fp; 
        fp = fopen("pink_panther.mp3","rb");

        while(1){
            unsigned char buff[500]={0};
            int readCount = fread(buff,1,500,fp);
            printf("Bytes read %d \n", readCount);        

            if(readCount > 0){
                printf("Sending \n");
                write(connfd, buff, readCount);
            }
            else{
                printf("Probably done\n");
                return 1;
            }
        }
        close(connfd);
        sleep(1);
    }


    return 0;
}