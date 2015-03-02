#ifndef __SPEAKV2__
#define __SPEAKV2__

#define SUCCESS 0

    typedef enum {TCP,UDP}conn_e;
    char *soundbuffer, *playbuffer;
    extern int remote_socket, audio_socket, remoteaudio_socket;
    extern unsigned int sample_rate;
    int acceptCall(char *choice);

#endif  
