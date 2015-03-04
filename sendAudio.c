#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <math.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "speak2.h"

 snd_pcm_t              *record, *playback;
 snd_pcm_uframes_t      frame_count;
 snd_pcm_uframes_t      frames_written;

 extern unsigned int    channels, sample_rate;
 extern unsigned int    resample, latency;
 extern unsigned int    totalSoundBytesSent, soundBytesSent;
 extern unsigned int    totalSoundBytesReceived, soundBytesReceived;
 extern unsigned int    totalControlBytesReceived, controlBytesReceived;


   int sendAudio(int remote_socket){

         int frames_read;
	 
         snd_pcm_open(&record , "default", SND_PCM_STREAM_CAPTURE, 0);
         snd_pcm_set_params(record, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED,
           channels, sample_rate, resample, latency);

         while(1){
           frames_read = snd_pcm_readi(record, soundbuffer, interval);
           soundBytesSent = send(remote_socket, soundbuffer, frames_read, 0);
           totalSoundBytesSent += soundBytesSent;
	 }

    return SUCCESS;
   }
