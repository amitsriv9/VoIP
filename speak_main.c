#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include "speak.h"



   int main(){
   int retVal = 1;
   char cmdBuffer[64]="",buffer[4096]="";
   unsigned int bytenos=512;
    initSetup();
    /*LOGIN*/
    while(1){
        printf(">");
        readInput(cmdBuffer);
        userCommandFactory(cmdBuffer, currentState);
	if(retVal==SUCCESS)
	   break;
    }
    while(1){
      printf(">");
    /*  PARAMS/ TALK/ ACCEPT/ DECLINE*/
        readInput(cmdBuffer);
        userCommandFactory(cmdBuffer, currentState);
	if(retVal==SUCCESS)
	   break;
    }

    while(1){
        busyWork(chunk_time);
    }
    return 0;
   }
