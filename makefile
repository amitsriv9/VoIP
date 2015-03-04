all: talk_server talk_client

talk_server: talk_server.o newLibrary.o errorrate.o recvAudio.o 
	gcc -g -Wall -lm -lasound talk_server.o newLibrary.o errorrate.o recvAudio.o -o talk_server 

talk_client: talk_client.o newLibrary.o errorrate.o sendAudio.o 
	gcc -g -Wall -lm -lasound talk_client.o newLibrary.o errorrate.o sendAudio.o -o talk_client 

talk_server.o:
	gcc -g -Wall -c talk_server.c

newLibrary.o:
	gcc -g -Wall -c newLibrary.c
 
errorrate.o:
	gcc -g -Wall -c errorrate.c

recvAudio.o:
	gcc -g -Wall -c recvAudio.c

sendAudio.o:
	gcc -g -Wall -c sendAudio.c

talk_client.c:
	gcc -g -Wall -c talk_client.c

clean:
	rm -rf *.o
