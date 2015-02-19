#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>


   int main(){
   int mysocket, remote_socket;
   struct sockaddr_in server, client;
   char buffer[4096]="";
   unsigned int bytenos=512;
   socklen_t addrlen;

    mysocket = socket(AF_INET, SOCK_STREAM,0);

    server.sin_family = AF_INET;
    server.sin_port = htons(6588);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(mysocket, (struct sockaddr*)&server, sizeof(server)) ;
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
