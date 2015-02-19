#ifndef __SPEAK__
#define __SPEAK__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <math.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include <fcntl.h>

   #define SUCCESS 0
   int _PLAYBACK = 0;
   int _RECORD = 0;
   int _IS_MUTE = 0;

   typedef enum state {INIT, LOGIN, TALK} state_e;

   socklen_t addrlen;
   int mysocket, remote_socket;
   struct sockaddr_in server, client;

   snd_pcm_t 		*record=NULL, *playback=NULL;
   snd_pcm_uframes_t 	frame_count;
   snd_pcm_uframes_t 	frames_written;
   unsigned int 	channels=1, sample_rate=8000; 
   unsigned int		resample=1, latency=500000;

   char 		*soundbuffer=NULL, *playbuffer=NULL, *buffer=NULL;

   unsigned short chunk_time;
   unsigned short sample_rate;
   state_e currentState = INIT;

   int setupSound(int sampleRate);
   int initCall();
   int logIn();
   int setParams(unsigned short rate,unsigned short sendTime );
   int changeState(state_e newstate);
   int sendRequest();
   int acceptRequest();
   int showMessage(int msgNo);
   int stopRecording();
   int startRecording();
   int endCall();
   int busyWork(int pushTime);
   int processRemote(char *buffer);



#endif
