#ifndef __SPEAK__
#define __SPEAK__

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
#include<netinet/in.h>



   #define SUCCESS 0
   int _PLAYBACK = 0;
   int _RECORD = 0;
   int _IS_MUTE = 0;

   typedef enum state {INIT, LOGIN, TALK} state_e;

   socklen_t addrlen;
   int mysocket, remote_socket;
   int myaudio_socket, remoteaudio_socket;
   struct sockaddr_in control_server, call_server, control_client, call_client;

   snd_pcm_t 		*record=NULL, *playback=NULL;
   snd_pcm_uframes_t 	frame_count;
   snd_pcm_uframes_t 	frames_written;
   unsigned int 	channels=1, sample_rate=8000; 
   unsigned int		resample=1, latency=500000;

   int 			totalSoundBytesReceived, totalSoundBytesSent, totalControlBytesReceived ;
   char 		*soundbuffer=NULL, *playbuffer=NULL, buffer[256]="";

   char 		name[64]="", remote_name[64]="";
  
   int			paramFlag=1;

 
   unsigned short chunk_time;
   state_e currentState = INIT;


   int userCommandFactory(char *command, state_e currentState);
   int setupSound(int sampleRate);
   int initCall();
   int logIn();
   int setParams(unsigned short rate,unsigned short sendTime );
   int parseParams(char* command );
   int getParamsAck(unsigned short rate,unsigned short sendTime );
   int changeState(state_e newstate);
   int sendRequest();
   int acceptRequest();
   int declineRequest();
   int showMessage(int msgNo);
   int stopRecording();
   int startRecording();
   int endCall();
   int endCallAck();
   int busyWork(int pushTime);
   int processRemote(char *command);
   int readInput(char * inbuffer);



#endif
