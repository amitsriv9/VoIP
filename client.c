#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>


   int main(){
   int mysocket ;
   unsigned int recv_no, sent_no;
   struct sockaddr_in server ;
   char buffer[4096]="";
   unsigned int bytenos =512;
   socklen_t addrlen;

    mysocket = socket(AF_INET, SOCK_STREAM,0);

    server.sin_family = AF_INET;
    server.sin_port = htons(6588);
    server.sin_addr.s_addr = INADDR_ANY;

    connect(mysocket, (struct sockaddr*)&server, sizeof(server));
    
    recv_no = recv(mysocket, buffer, bytenos,0);

    if(recv_no > 0){
    	printf("from the server\t%s", buffer);
    	memset(buffer,0,sizeof(buffer));
    	sprintf(buffer, "okay bye!");
   
 	sent_no = send(mysocket, buffer, bytenos,0);
    }

    close(mysocket);
    return 0;
   }
