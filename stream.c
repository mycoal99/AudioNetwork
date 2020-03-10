#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#define DR_FLAC_IMPLEMENTATION
#include "./extras/dr_flac.h" /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "./extras/dr_mp3.h" /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "./extras/dr_wav.h" /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include "./miniaudio.h"

// void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
// {
//     ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
//     if (pDecoder == NULL)
//     {
//         return;
//     }

//     ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

//     (void)pInput;
// }

// void *playback(void *songFile)
// {
//     sleep(5);
//     char* argv;
//     argv = (char *)songFile;
//     ma_result result;
//     ma_decoder decoder;
//     ma_device_config deviceConfig;
//     ma_device device;

//     // if (argc < 2)
//     // {
//     //     printf("No input file.\n");
//     //     return -1;
//     // }

//     result = ma_decoder_init_file(argv, NULL, &decoder);
//     if (result != MA_SUCCESS)
//     {
//         pthread_exit((int *)-2);
//     }

//     deviceConfig = ma_device_config_init(ma_device_type_playback);
//     deviceConfig.playback.format = decoder.outputFormat;
//     deviceConfig.playback.channels = decoder.outputChannels;
//     deviceConfig.sampleRate = decoder.outputSampleRate;
//     deviceConfig.dataCallback = data_callback;
//     deviceConfig.pUserData = &decoder;

//     printf("%llu", decoder.readPointer);

//     if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
//     { //initializes device with device config
//         printf("Failed to open playback device.\n");
//         ma_decoder_uninit(&decoder);
//         pthread_exit((int*)-3);
//     }

//     if (ma_device_start(&device) != MA_SUCCESS)
//     { //starts playback
//         printf("Failed to start playback device.\n");
//         ma_device_uninit(&device);
//         ma_decoder_uninit(&decoder);
//         pthread_exit((int *)-4);
//     }

//     while (1)
//     {
//         printf("Press enter to pause");
//         getchar();
//         ma_device_stop(&device);
//         printf("Press enter to resume");
//         getchar();
//         ma_device_start(&device);
//     }

//     pthread_exit((int *)0);
// }

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("incorrect usage\nCorrect Usage: \'./stream <filename>\'");
    }

    char done[1] = "q";
    int sockfd = 0, bytesReceived = 0;
    char recvBuff[500];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Could not create socket\n");
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081); // port
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    FILE *fp;
    fp = fopen("test.mp3", "w"); 
    if(NULL == fp){
        printf("Unable to open file");
        return 1;
    }

    while((bytesReceived = read(sockfd, recvBuff, 500)) > 0){
        // printf("Bytes received %c\n",bytesReceived);    
        fwrite(recvBuff, 1,bytesReceived,fp);
        // printf("%s\n", recvBuff);
    }

    write(sockfd, done, 1);

    return 0;
}