#ifndef __SPEAKSS__
#define __SPEAKSS__

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



   #define SUCCESS 0
   #define ACCEPT 15
   #define DECLINE 18
   extern int _PLAYBACK ;
   extern int _RECORD ;
   extern int _IS_MUTE ;

   typedef enum state {INIT, LOGIN, TALK} state_e;

   extern socklen_t addrlen;
   extern int mysocket, remote_socket;
   extern int myaudio_socket, remoteaudio_socket;
   extern struct sockaddr_in control_server, call_server, control_client, call_client;

   extern snd_pcm_t 		*record, *playback;
   extern snd_pcm_uframes_t 	frame_count;
   extern snd_pcm_uframes_t 	frames_written;
   extern unsigned int 	channels, sample_rate; 
   extern unsigned int		resample, latency;

   int 			totalSoundBytesReceived, totalSoundBytesSent, totalControlBytesReceived ;
   char 		*soundbuffer, *playbuffer, buffer[256], r_buffer[256];

   char 		name[64], remote_name[64];
  
   int			paramFlag;

 
   unsigned short chunk_time;
   state_e currentState ;


   int userCommandFactory(char *command, state_e currentState);
   int setupSound(int sampleRate);
   int initSetup();
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
