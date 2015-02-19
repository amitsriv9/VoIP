#include "speak.h"

   int userCommandFactory(char *command, state_e currentState){
   /*   INIT  - default
    *   LOGIN   
    *   PARAMS  8000/16000  20 40 80 100 200 500 1000
    *   TALK    username
    *   MUTE/UNMUTE
    *   STOP    
    *   LOGOUT
    */
    if(commmand == NULL){
        return -2;
    }
    if( !strncmp(command,"LOGIN",5)||
    		!strncmp(command,"PARAMS",6)||
    			!strncmp(command,"CALL",4)||
    				!strncmp(command,"MUTE",4)||
    					!strncmp(command,"UNMUTE",6)||
   	 		 			!strncmp(command,"STOP",4)||
   			 				!strncmp(command,"LOGOUT",6)
   			 					!strncmp(command,"ACCEPT",6)){

	if(currentState==INIT){
		if(!strncmp(command, "LOGIN", 5)){
		     logIn();
		}
	}
 
	else if(currentState==LOGIN){
		if(!strncmp(command, "PARAMS", 6)){
		     setParams();
		}
			if(!strncmp(command, "LOGOUT", 6)){
			      changeState();
			}
				if(!strncmp(command, "TALK", 4)){
				      sendRequest();  
				}
					if(!strncmp(command, "ACCEPT", 6)){
				               acceptRequest();  
					}
	}

	else if(currentState==TALK){
		if(!strncmp(command, "MUTE)", 4)){
		    stopRecording(); 
		}
			if(!strncmp(command, "UNMUTE)", 4)){ 
			    startRecording();
			}
				if(!strncmp(command, "STOP", 4)){
				     endCall();
				}
	}

        else{
		return -1;
	}


    }

    else{
	return -1;
    }

   }

    int setupSound(int sampleRate){

	if(sampleRate==16000){
	    sample_rate = 16000;
	}
	else{
	    sample_rate = 8000;
	}

 	snd_pcm_open(&record , "default", SND_PCM_STREAM_CAPTURE, 0);
 	snd_pcm_set_params(record, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED,
   			channels, sample_rate, resample, latency); 

 	snd_pcm_open(&playback , "default", SND_PCM_STREAM_PLAYBACK, 0);
 	snd_pcm_set_params(playback, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED,
   			channels, sample_rate, resample, latency); 


    }
   int initCall(){

   	mysocket = socket(AF_INET, SOCK_STREAM,0);
   	myaudio_socket = socket(AF_INET, SOCK_STREAM,0);

    	control_server.sin_family = AF_INET;
    	control_server.sin_port = htons(6588);
    	control_server.sin_addr.s_addr = INADDR_ANY;

    	call_server.sin_family = AF_INET;
    	call_server.sin_port = htons(6688);
    	call_server.sin_addr.s_addr = INADDR_ANY;

    	bind(mysocket, (struct sockaddr*)&control_server, sizeof(control_server)) ;
    	bind(myaudio_socket, (struct sockaddr*)&call_server, sizeof(call_server)) ;

	return SUCCESS;
   }

   int logIn(){
       	listen(mysocket,5); 
       	listen(myaudio_socket,5); 

	state_e temp=LOGIN;
	changeState(temp);
	remote_socket = accept(mysocket, (struct sockaddr*)&control_client, &addrlen);
	remoteaudio__socket = accept(myaudio_socket, (struct sockaddr*)&call_client, &addrlen);
	return SUCCESS;
   }
   int setParams(unsigned short rate,unsigned short sendTime ){
 	sample_rate=rate;
	chunk_time=sendTime;

	state_e temp=TALK;
	changeState(temp);
	setupSound(rate);

	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"PARAMS %s|%d|%d", name, rate, sendTime);
	send(remote_socket, buffer, strlen(buffer),0);

	return SUCCESS;
   }
   int getParamsAck(unsigned short rate,unsigned short sendTime ){
 	sample_rate=rate;
	chunk_time=sendTime;

	state_e temp=TALK;
	changeState(temp);
	setupSound(rate);

	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"PARAMS_ACK %s", name);
	send(remote_socket, buffer, strlen(buffer),0);

	return SUCCESS;
   }
   int changeState(state_e newstate){
	currentState = newState;
	return SUCCESS;
   }
   int sendRequest(){
	sprintf(buffer,"TALK %s", name);
	send(remote_socket, buffer, strlen(buffer),0);
	return SUCCESS;
   }
   int acceptRequest(){
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"ACCEPT %s", name);
	send(remote_socket, buffer, strlen(buffer),0);

	state_e temp=TALK;
	changeState(temp);
	return SUCCESS;
   }
   int showMessage(int msgNo){

	switch(msgNo){
	case 1:printf("Incoming Call from %s\n",remote_name);
		break;
	case 2:printf("Call closed by %s\n",remote_name);
		break;
	case 3:printf("MUTE ON\n");
		break;
	case 4:printf("MUTE OFF\n");
		break;
	}
	return SUCCESS;
   }
   int stopRecording(){
	memset(buffer,0,sizeof(buffer));
        sprintf(buffer,"MUTE_ON");
	IS_MUTE=0;
	return SUCCESS;
   } 
   int startRecording(){
	_IS_MUTE=1;
	return SUCCESS;
   } 
   int endCall(){
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"STOP %s", name);
	send(remote_socket, buffer, strlen(buffer),0);
	_PLAYBACK=1;
	_RECORD=1;	

	state_e temp=LOGIN;
	changeState(temp);
	return SUCCESS;
   }
   int endCallAck(){
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"STOP_ACK %s", name);
	send(remote_socket, buffer, strlen(buffer),0);
	_PLAYBACK=1;
	_RECORD=1;	

	state_e temp=LOGIN;
	changeState(temp);
	return SUCCESS;
   }

   int busyWork(int pushTime){

	fd_set ctrl_fds, listen_fds, input_fds;
	struct timeval tv1, tv2;
	int retval1, retval2, retval3;
	int soundBytesReceived=0, soundBytesSent=0, controlBytesReceived=0;
	int frames_read=0, frames_played=0;

	tv1.tv_sec=0;
	tv1.tv_usec=500;

	tv2.tv_sec=0;
	tv2.tv_usec=500;

	tv3.tv_sec=0;
	tv3.tv_usec=500;

        FD_ZERO(&input_fds);
	FD_SET(stdin,&input_fds);

        FD_ZERO(&ctrl_fds);
	FD_SET(remote_socket, &ctrl_fds);

        FD_ZERO(&listen_fds);
	FD_SET(remoteaudio_socket, &listen_fds);

        //record
        if((pushTime==20)||
        	(pushTime==40)||
        		(pushTime==80)||
        			(pushTime==100)||
        				(pushTime==200)||
        					(pushTime==500)){

	    interval = pushTime*(sample_rate/1000);

	}

	else{
		return -1;
	}

	soundbuffer = (char*) malloc(sample_rate* 10 * sizeof(char));
	playbuffer  = (char*) malloc(sample_rate* 10 * sizeof(char));

	if(soundbuffer==NULL || playbuffer ==NULL){
		exit(1);
	}

	while(1){

           frames_read = snd_pcm_readi(record, soundbuffer + totalread, interval);
	   soundBytesSent = send(remote_socket, soundbuffer + totalread, frames_read, 0);

	   totalSoundBytesSent += soundBytesSent;


	   retval1 = select(remoteaudio_socket, &listen_fds, NULL,NULL, &tv1);
	   if(retval1 != -1){
	         soundBytesReceived = recv(remoteaudio_socket, playbuffer, sizeof(playbuffer), 0);
           	 frames_played += snd_pcm_writei(playback, playbuffer, frame_count);

	         totalSoundBytesReceived += soundBytesReceived;
	   }


	   retval2 = select(remote_socket, &ctrl_fds,NULL, NULL, &tv2);
	   if(retval2 != -1){
            	 controlBytesReceived = recv(remote_socket, buffer, sizeof(playbuffer), 0);
	   	 processRemote(buffer);

	   	 totalControlBytesReceived += controlBytesReceived; 
	   }


	   retval3 = select(1, &input_fds,NULL, NULL, &tv3);
	   if(retval3 != -1){
	         userCommandFactory();
	   }


           FD_ZERO(&input_fds);
	   FD_SET(stdin,&input_fds);

           FD_ZERO(&ctrl_fds);
	   FD_SET(remote_socket, &ctrl_fds);

           FD_ZERO(&listen_fds);
	   FD_SET(remoteaudio_socket, &listen_fds);

	   tv1.tv_sec=0;
	   tv1.tv_usec=500;

	   tv2.tv_sec=0;
	   tv2.tv_usec=500;

	   tv3.tv_sec=0;
	   tv3.tv_usec=500;

	}  // end of while


	return SUCCESS;
   }

   int processRemote(char *command){
    unsigned short rate, sendTime;

    if(command==NULL){
	return -1;
    }	  

    if( !strncmp(command,"PARAM_ACK",9)||
    		!strncmp(command,"PARAMS",6)||
    			 !strncmp(command,"STOP",4)||
    				!strncmp(command,"STOP_ACK",8)||
    					!strncmp(command,"CALL",4)||
    						!strncmp(command,"CALL_ACK",8)||
    							!strncmp(command,"UNKNOWN",7)){

	if(!strncmp(command,"CALL",4)){
		showMessage(1);	
	}
	else if (!strncmp(command,"PARAMS",6)){
		parseParams(command, &rate, &sendTime);
		getParamsAck(rate, sendTime);
	}
	else if (!strncmp(command,"STOP",4)){
		endCallAck();
	}
    }

    return SUCCESS;
   }
