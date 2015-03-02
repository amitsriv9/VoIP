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


 snd_pcm_t 		*record, *playback;
 snd_pcm_uframes_t      frame_count;
 snd_pcm_uframes_t      frames_written;
 unsigned int           channels=1, sample_rate=8000;
 unsigned int           resample=1, latency=500000;
 unsigned int           totalSoundBytesSent=0, soundBytesSent=0;
 unsigned int 		totalSoundBytesReceived=0, soundBytesReceived=0;
 unsigned int 		totalControlBytesReceived=0, controlBytesReceived=0;
 /*****************************
 *  readInput
 *  input
 *  output
 *
 ******************************/

   int readInput(char * inbuffer){

     char in, *ptr=NULL;

     if(inbuffer==NULL){
	 return -1;
     }

     ptr = inbuffer;
     while((in = getchar())!='\n'){
	*ptr++ = in;
     }
     *ptr='\0';

     return SUCCESS;
   }
 /*****************************
 *  accept
 *  input
 *  output
 *
 ******************************/

   int acceptCall(char *choice){

    if (choice == NULL)
        return -1;
    if(!strncmp(choice,"y",1))
       return 0;
    if(!strncmp(choice,"n",1))
       return 1;

    return 1;
   }

 /*****************************
 *   parseParams
 *   input
 *   output
 *
 ******************************/

   int parseParams(char *command){

      int port, samplerate,chunksize,loss, count=0;
      char *temp=NULL, buff[64]="";
      temp = strtok(command, ",");

      if(temp!=NULL){

	 while(temp!=NULL){

             strncpy(buff,temp, sizeof(buff));
	     count+=1;

	     switch(count){
	     	case 1: sample_rate = atoi(buff); break;
	     	case 2: chunksize = atoi(buff); break;
	     }
	     memset(buff, 0, sizeof(buff));
             temp = strtok(command, "|");
         }
	 printf("%d%d\n",sample_rate, chunksize); 

      }	
      setAll(&sample_rate);
      return SUCCESS;
    }

 /*****************************
 *  setall 
 *  input 
 *  output
 *
 ******************************/

    int setAll(int *samplerate){

	if(samplerate!=NULL){
	    sample_rate = *samplerate;
	}
	 snd_pcm_open(&record , "default", SND_PCM_STREAM_CAPTURE, 0);
	 snd_pcm_set_params(record, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED,
                        channels, sample_rate, resample, latency);

	 snd_pcm_open(&playback , "default", SND_PCM_STREAM_PLAYBACK, 0);
	 snd_pcm_set_params(playback, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED,
                        channels, sample_rate, resample, latency);
     return SUCCESS;
    }

 /*****************************
 * busywork
 * intput
 * output
 *
 ******************************/

    int busyWork(int *chunksize, char *control_buffer){

	fd_set ctrl_fds, listen_fds, input_fds;
	struct timeval tv1, tv2, tv3;
	int retval1, retval2, retval3;
	int soundBytesReceived=0, soundBytesSent=0, controlBytesReceived=0;
	unsigned int totalread=0, frames_read=0, frames_played=0;
	unsigned int interval;
        char in,*ptr, localbuffer[65]="";
        int pushwindow =40;

        if(chunksize!=NULL)
	   pushwindow = *chunksize;

        //record
        if((pushwindow==20)||
        	(pushwindow==40)||
        		(pushwindow==80)||
        			(pushwindow==1000)||
        				(pushwindow==200)||
        					(pushwindow==500))
        {
	    interval = pushwindow *(sample_rate/1000);
	}

	else
        {
	    pushwindow = 40;
	    interval = pushwindow *(sample_rate/1000);
	}

	soundbuffer = (char*) malloc(sample_rate* 10 * sizeof(char));
	playbuffer  = (char*) malloc(sample_rate* 10 * sizeof(char));

	if(soundbuffer==NULL || playbuffer ==NULL){
		printf("mem alloc failed for audio buffer\n");
		exit(1);
	}

	while(1){
		tv1.tv_sec=0;
		tv1.tv_usec=500;
		tv2.tv_sec=0;
		tv2.tv_usec=500;
		tv3.tv_sec=0;
		tv3.tv_usec=500;

        	FD_ZERO(&input_fds);
		FD_SET(0,&input_fds);
        	FD_ZERO(&ctrl_fds);
		FD_SET(remote_socket, &ctrl_fds);
        	FD_ZERO(&listen_fds);
		FD_SET(remoteaudio_socket, &listen_fds);

           	frames_read = snd_pcm_readi(record, soundbuffer + totalread, interval);
	   	soundBytesSent = send(remote_socket, soundbuffer + totalread, frames_read, 0);

	   	totalSoundBytesSent += soundBytesSent;


	   	retval1 = select(remoteaudio_socket+1, &listen_fds, NULL,NULL, &tv1);

	   	if(retval1 > 0 ){
	         	soundBytesReceived = recv(remoteaudio_socket, playbuffer, sizeof(playbuffer), 0);
           	 	frames_played += snd_pcm_writei(playback, playbuffer, frame_count);

	         	totalSoundBytesReceived += soundBytesReceived;
	   	}


	   	retval2 = select(remote_socket+1, &ctrl_fds,NULL, NULL, &tv2);

	   	if(retval2 > 0){
            	 	controlBytesReceived = recv(remote_socket, control_buffer, sizeof(control_buffer), 0);
	   	 	//processRemote(buffer);
			if(!strncmp(control_buffer, "STOP",4)){
			    memset(control_buffer, 0, 512);
			    sprintf(control_buffer, "STOP_ACK");
			    send(remote_socket, control_buffer, strlen(control_buffer),0);
	   	 	    totalControlBytesReceived += controlBytesReceived;
			    break; 
			}
	   	 	totalControlBytesReceived += controlBytesReceived; 
	   	}

	   	/* the user write something on the prompt lets read it*/
	   	/*retval3 = select(1, &input_fds,NULL, NULL, &tv3);

	   	if(retval3 != -1){
			memset(localbuffer, 0, sizeof(localbuffer));
			ptr = localbuffer;
     			while((in = getchar())!='\n'){
				*ptr++ = in;
     			}
     		*ptr='\0';
	        userCommandFactory(localbuffer, currentState);
	   	}	
		*/

	}  // end of while
	return SUCCESS;
   }

