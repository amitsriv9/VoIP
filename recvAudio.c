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
 extern int	        max_drop_count;

  // set the sample rate and the framecount 
  int recvAudio(int remoteaudio_socket, int loss, char* audio){
         int frames_played=0;
         char* soundbuffer=NULL;
	 soundbuffer = audio;

         snd_pcm_open(&playback , "default", SND_PCM_STREAM_PLAYBACK, 0);
         snd_pcm_set_params(playback, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED,
                        channels, sample_rate, resample, latency);

	 while(1){
             soundBytesReceived = recv(remoteaudio_socket, soundbuffer, sizeof(playbuffer), 0);
             totalSoundBytesReceived += soundBytesReceived;

	     if(dropPackets(loss) || max_drop_count < 1)
             	frames_played += snd_pcm_writei(playback, soundbuffer, frame_count);
	 }
         
  return SUCCESS;
  }
