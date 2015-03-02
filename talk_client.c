#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include "speak2.h"


    int mysocket, remote_socket , myport, audio_socket, remoteaudio_socket, audio_port, losspercent;
    int main(int argc , char **argv){

    unsigned int recv_no, sent_no;
    struct sockaddr_in server , talk;
    char buffer[512]="", param_buffer[512]="";
    char *extract=NULL;
    int samplingrate=8000, chunksize=20;
    socklen_t addrlen;
    conn_e connection = TCP;

    if(argc < 3){
       exit(1);
    }


    readInput(buffer);
    if(!strncmp(buffer, "PARAMS",6))
	extract = strtok(buffer+6, ",");
	if(extract!=NULL){
	 strncpy(param_buffer, extract, sizeof(param_buffer));
	 sample_rate = atoi(param_buffer);
	 extract = strtok(NULL, ",");
	 if(extract==NULL){
	    memset(param_buffer, 0, sizeof(param_buffer));
	    strncpy(param_buffer, extract, sizeof(param_buffer));
	    chunksize = atoi(param_buffer);	
 	 }
	}
    strncpy(param_buffer, argv[1], sizeof(param_buffer));
    myport = atoi(param_buffer);
    memset(param_buffer,0,sizeof(param_buffer));
    audio_port  = myport+1000; 
    if(!strncmp(argv[2],"UDP",3)){
        connection = UDP;
    }
    losspercent = atoi(argv[3]);

    if(connection == TCP){
        mysocket = socket(AF_INET, SOCK_STREAM,0);
        audio_socket = socket(AF_INET, SOCK_STREAM,0);
    }
    else{
        mysocket = socket(AF_INET, SOCK_DGRAM,0);
        audio_socket = socket(AF_INET, SOCK_DGRAM,0);
    }
    printf("remote port is %d\n", myport);
    server.sin_family = AF_INET;
    server.sin_port = htons(myport);
    server.sin_addr.s_addr = INADDR_ANY;

    talk.sin_family = AF_INET;
    talk.sin_port = htons(audio_port);
    talk.sin_addr.s_addr = INADDR_ANY;
    printf("Going to connect..\n");
    connect(mysocket, (struct sockaddr*)&server, sizeof(server));
    
    recv_no = recv(mysocket, buffer, sizeof(buffer),0);
    if(!strncmp(buffer, "ACCEPT", 6)){
        printf("got accept..\n");
	memset(buffer, 0, sizeof(buffer));  
        sprintf(buffer,"PARAMS %d,%d",sample_rate,chunksize);
        printf("sending params..\n");
        send(mysocket, buffer, strlen(buffer),0); 
	memset(buffer, 0, sizeof(buffer));  
        recv_no = recv(mysocket, buffer, sizeof(buffer),0);
    }

    if(!strncmp(buffer, "PARAMS_ACK", 10))
    {
        if(!connect(audio_socket, (struct sockaddr*)&talk, sizeof(talk)))
	    printf("Call is connected\n");
	else
	    exit(1);
    }
    setAll(&samplingrate);
    busyWork(&chunksize, param_buffer);
    close(mysocket);
    return 0;
   }
