#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>


   //typedef enum state{INIT, LOGIN, PARAMS, TALK, MUTE, STOP, LOGOUT} state_e;

   int main(){
   char buffer[4096]="";
   unsigned int bytenos=512;
   // INIT-------------------------------------------------------
   socklen_t addrlen;
   int mysocket, remote_socket;
   int myaudio_socket, remoteaudio_socket;
   struct sockaddr_in server, client;

    mysocket = socket(AF_INET, SOCK_STREAM,0);
    myaudio_socket = socket(AF_INET, SOCK_STREAM,0);

    server.sin_family = AF_INET;
    server.sin_port = htons(6588);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(mysocket, (struct sockaddr*)&server, sizeof(server)) ;
   //----------------------------------------------------------
    readInput();
    userCommandFactory();

    int retval, retval2;
    fd_set input, remote_req;
    struct timeval tt1,tt2;
    tt1.tv_sec  = 0;
    tt1.tv_usec = 1000;
    tt2.tv_sec  = 0;
    tt2.tv_usec = 1000;

    while(1){

        FD_ZERO(&input);
        FD_SET(0, &input);    

        FD_ZERO(&remote_req);
        FD_SET(remote_socket, &remote_req);
    
	retval =  select(remote_socket+1, &remote_req, NULL, NULL, tt );
	if(retval !=-1){
		processRemote();
		if(!paramFlag)
		    break;
	}

	retval2 = select(1, &input, NULL, NULL, tt2 );
	if(retval2 !=-1){
    		readInput();
    		userCommandFactory();
		if(!paramFlag)
		    break;
	}

        tt1.tv_sec  = 0;
        tt1.tv_usec = 1000;
        tt2.tv_sec  = 0;
        tt2.tv_usec = 1000;
    }

    listen(mysocket, 5);
    addrlen = sizeof(client);
    remote_socket = accept(mysocket, (struct sockaddr*)&client, &addrlen);
    printf("remote socket is %d\n", remote_socket);
 
    sprintf(buffer,"Hi there this is the server!!\nGoof baaye!"); 
    send(remote_socket, buffer, bytenos,0);
    memset(buffer,0,sizeof(buffer));
    recv(remote_socket, buffer, bytenos,0);
    printf("%s\n", buffer);
    close(mysocket);

    return 0;
   }
