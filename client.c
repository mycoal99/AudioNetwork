#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    int sockfd = 0, bytesReceived = 0;
    char recvBuff[500];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Could not create socket\n");
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // port
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    FILE *fp;
    fp = fopen("test.mp3", "w"); 
    if(NULL == fp){
        printf("Unable to open file");
        return 1;
    }

    while((bytesReceived = read(sockfd, recvBuff, 500)) > 0){
        printf("Bytes received %c\n",bytesReceived);    
        fwrite(recvBuff, 1,bytesReceived,fp);
        printf("%s\n", recvBuff);
    }

    return 0;
}