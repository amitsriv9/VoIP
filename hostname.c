#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
   int main(int argc , char** argv){
	    char str[INET_ADDRSTRLEN];
            char *ptr=*++argv, **pptr;
	    struct hostent *hptr=NULL;
	    struct addrinfo hints, *result, *k;


	    hptr = gethostbyname(ptr);
   	    memset(&hints, 0, sizeof(struct addrinfo));
    	    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    	    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    	    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    	    hints.ai_protocol = 0;          /* Any protocol */
    	    hints.ai_canonname = NULL;
    	    hints.ai_addr = NULL;
    	    hints.ai_next = NULL;
	    getaddrinfo(ptr,*++argv,&hints,&result);

	    for(k=result;k->ai_next!=NULL;k++){
		printf("address %s\n",inet_ntop(k->ai_family,k->ai_addr,str, sizeof(str)));

	    }

	    if(hptr==NULL){
		exit(1);
    	    }
	    pptr = hptr->h_aliases;
	    while(*pptr++!=NULL){
		printf("aliases %s\n",*pptr);

	    }
	    printf("officialname %s\n", hptr->h_name);
	    switch(hptr->h_addrtype){

	    case AF_INET6:
		  pptr = hptr->h_addr_list;
		  for(;*pptr != NULL;*pptr++){
			printf("address %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
		       
		  }
		  break;
	    }
            return 0;
   }
