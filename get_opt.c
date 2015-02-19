#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
  typedef enum protocol{TCP, UDP} protocol_e;
  extern char* optarg;
  int main(int argc , char ** argv){


   int opt, sample_rate, send_rate, detection;
   protocol_e protocol=TCP;

    while( (opt = getopt(argc, argv, "spdz"))!=-1){

          switch(opt){
	  case 's':  sample_rate = atoi(argv[optind]); break;

	  case 'p':  if(! strncmp(argv[optind], "UDP",3))
			protocol = UDP;
		     break;

	  case 'd':  if(!strncmp(argv[optind], "ON", 2))
		        detection = 0;
		     break;

	  case 'z':  send_rate = atoi(argv[optind]);
		     break;

	  default: printf("no match found\n");
          }
          printf("sample rate %d send_rate %d\n", sample_rate, send_rate);
    }

  return 0;
  }
