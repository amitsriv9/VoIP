#include "speak.h"



   int main(){

    unsigned int bytenos=512;
    readInput(buffer);
    userCommandFactory(buffer, currentState);

    int retval, retval2;
    fd_set input, remote_req;
    struct timeval tt1,tt2;
    tt1.tv_sec  = 0;
    tt1.tv_usec = 1000;
    tt2.tv_sec  = 0;
    tt2.tv_usec = 1000;
    printf("Connected to Remote Host\n");
    while(1){

        FD_ZERO(&input);
        FD_SET(0, &input);    

        FD_ZERO(&remote_req);
        FD_SET(remote_socket, &remote_req);
    
	retval =  select(remote_socket+1, &remote_req, NULL, NULL, tt );
	if(retval !=-1){
		processRemote(buffer);
		if(!paramFlag)
		    break;
	}

	retval2 = select(1, &input, NULL, NULL, tt2 );
	if(retval2 !=-1){
    		readInput(buffer);
    		userCommandFactory(buffer);
		if(!paramFlag)
		    break;
	}

        tt1.tv_sec  = 0;
        tt1.tv_usec = 1000;
        tt2.tv_sec  = 0;
        tt2.tv_usec = 1000;
    }
    
    busyWork();

    return 0;
   }
