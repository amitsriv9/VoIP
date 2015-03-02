#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include "speak2.h"


   int mysocket, remote_socket, myport, audioport,audio_socket, remoteaudio_socket;
   int main(int argc , char **argv){

   int losspercent = 0;
   struct sockaddr_in server, client, talk;
   char buffer[512]="", param_buffer[512]="", *temp=NULL;

   int samplingrate=8000, chunksize=20;

   socklen_t addrlen;
   conn_e connection = TCP;

    if(argc < 3){
       exit(1);
    }

    myport = atoi(argv[1]);
    audioport = myport+1000;

    if(!strncmp(argv[2],"UDP",3)){
        connection = UDP;
    }
    losspercent = atoi(argv[3]);

    if(connection == TCP){
        mysocket = socket(AF_INET, SOCK_STREAM,0);
        audio_socket = socket(AF_INET, SOCK_STREAM,0);
    }
    else{
        mysocket = socket(AF_INET,SOCK_DGRAM,0);
        audio_socket = socket(AF_INET,SOCK_DGRAM,0);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(myport);
    server.sin_addr.s_addr = INADDR_ANY;

    talk.sin_family = AF_INET;
    talk.sin_port = htons(audioport);
    talk.sin_addr.s_addr = INADDR_ANY;

    bind(mysocket, (struct sockaddr*)&server, sizeof(server)) ;
    bind(audio_socket, (struct sockaddr*)&talk, sizeof(talk)) ;

    /*this is only for TCP UDP doesnt care*/
    if(connection == TCP){
      listen(mysocket, 5);
      printf("listening\n");
      addrlen = sizeof(client);
      remote_socket = accept(mysocket, (struct sockaddr*)&client, &addrlen);
      printf("Remote socket is %d\nAccept?(y/n)\n", remote_socket);
      readInput(buffer);
    }
 
    if (0==acceptCall(buffer)){
       /*  send the acceptance to remote host*/
       sprintf(buffer,"ACCEPT %d",audioport);
       send(remote_socket, buffer, strlen(buffer),0);   


       /*and receive parameters to setup the call*/
       recv(remote_socket, buffer, sizeof(buffer),0);
       if(!strncmp(buffer, "PARAMS", 6))
       {
	   printf("%s\n", buffer);
	   temp  = strtok(buffer+6,",");
	   if(temp!=NULL){

		memset(param_buffer,0,sizeof(param_buffer));
		strncpy(param_buffer,temp,sizeof(param_buffer));
	        printf("%s\n", param_buffer);
		samplingrate = atoi(param_buffer);		/*  the bitrate  8000/16000 */
	        temp  = strtok(NULL,",");

	        if(temp==NULL){

		   memset(param_buffer,0,sizeof(param_buffer));
		   strncpy(param_buffer,temp,sizeof(param_buffer));
		   chunksize = atoi(param_buffer);       /* chunksize of the audio to be sent 20,40,100,200,500 ms*/

		}
  	   }
 	printf("Params received %d %d\n", samplingrate,chunksize);
       }
       sprintf(buffer,"PARAMS_ACK AP%d",audioport);
       send(remote_socket, buffer, strlen(buffer),0);   
       listen(audio_socket, 5);
       remoteaudio_socket = accept(audio_socket, (struct sockaddr*)&client, &addrlen);

    setAll(&samplingrate);     
    busyWork(&chunksize, param_buffer);

    }
    close(mysocket);
    return 0;
   }
