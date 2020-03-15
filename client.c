#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
#define DR_FLAC_IMPLEMENTATION
#include "./extras/dr_flac.h" /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "./extras/dr_mp3.h" /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "./extras/dr_wav.h" /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include "./miniaudio.h"

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

    (void)pInput;
}


int main(void)
{
    //create vars for communicating with stream.c
    int fd[2];
    pipe(fd);
    char pipefd[12];
    snprintf(pipefd,12,"%i",fd[0]);

    //create child process and have it execute stream.c
    int PID = fork();
    if (!PID){
        char* argv_list[] = {"./stream", "test.mp3", pipefd, NULL} ;
        execv("./stream", argv_list);
    }   

    //Program waits until calculated delay time stamp from server
    // printf("Start time = %li\n",startTime);
    // while(timeOfDay != startTime){
    //     gettimeofday(&time,NULL);
    //     timeOfDay = ((time.tv_sec * 1000) + (time.tv_usec / 1000))%10000; //Modulo used to simplify the timestamp
    //     printf("Time of day = %li\n",timeOfDay);
    // }

    //client process yields to stream process
    sched_yield();
    //sleep to make sure file is partially written before read begins
    sleep(5);

    //this is all the example stuff that does playback
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file("test.mp3", NULL, &decoder);
    if (result != MA_SUCCESS) {
        return -2;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    printf("%llu\n",decoder.readPointer);

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) { //initializes device with device config
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) { //starts playback
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    //allows user response to pause, resume, volume up, volume down, quit - respectively
    char input;
    while(1){
        scanf("%c",&input);
        if(input == 'p') {
            ma_device_stop(&device);
        }
        if(input == 'r') {
            ma_device_start(&device);
        }
        if(input == 'u') {
            float up;
            ma_device_get_master_volume(&device, &up);
            up += 0.1;
            if (up > 1)
                up = 1;
            ma_device_set_master_volume(&device, up);
        }
        if(input == 'd') {
            float down;
            ma_device_get_master_volume(&device, &down);
            down -= 0.1;
            if (down < 0)
                down = 0;
            ma_device_set_master_volume(&device, down);
        }
        if(input == 'q') {
            write(fd[1], &input, sizeof(&input));
            ma_device_uninit(&device);
            ma_decoder_uninit(&decoder);
            close(fd[0]);
            close(fd[1]);
            remove("test.mp3");
            break;
        }

    }

    // pthread_t playbackThread;
    // const char* arglist = malloc(42);
    // arglist = "test.mp3";
    // pthread_create(&playbackThread, NULL, playback, (void*)&arglist);
    
    // fprintf(stderr, "%d",pthread_join(playbackThread, NULL));
    return 0;
}