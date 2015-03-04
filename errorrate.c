#include<stdlib.h>
#include<time.h>

/*
 *                chunk size		8000hz			16000hz
 *                20  (50)		160  frames		  320  f
 *                40  (25) 		320  f			  640
 *                80  (12) 		666  f			 1333
 *                200 (5)		1600 f			 3200
 *                500 (2)		4000 f			 8000
 *                1000(1)		8000 f			16000
 */                


  /* 
   * setSampleSize
   * input percentage loss, and time chunk to be sent
   *
   *
   */
    extern int framecount, sample_rate;
    int samplesize=0, max_drop_count=0;
    int setSampleSize(int percent, int chunksize){

    	switch(chunksize){
     	   case 20: 
		samplesize = 50;
		if(sample_rate==8000)
			framecount=160;
	  	else
			framecount=320;  
			break;
     	   case 40: 
		samplesize = 25;  
		if(sample_rate==8000)
			framecount=320;
	  	else
			framecount=640;  
			break;
     	   case 80: 
		samplesize = 12;  
		if(sample_rate==8000)
			framecount=666;
	  	else
			framecount=1333;  
			break;
     	   case 200: 
		samplesize = 10;  
		if(sample_rate==8000)
			framecount=1600;
	  	else
			framecount=3200;  
			break;
     	   case 500:
		samplesize = 10;  
		if(sample_rate==8000)
			framecount=4000;
	  	else
			framecount=8000;  
			break;
     	   case 1000: 
		samplesize = 10;  
		if(sample_rate==8000)
			framecount=8000;
	  	else
			framecount=16000;  
			break;
    	   default: 
		samplesize = 25; 
			break;
        }

    max_drop_count = (samplesize*percent) / 100; 
    return 0;
    }

    int dropPackets(int percent){
	int chance=0;

	srand(time(NULL));

        chance = rand() % 20 + 1;
	if(chance < 5){

	   switch(percent){

	      case 5:
		max_drop_count-=1; 
		if(chance == 1)
		    return 0;	
	      case 10:	
		max_drop_count-=1; 
		if(chance <= 2)
		    return 0;	
	      case 15:	
		max_drop_count-=1; 
		if(chance <= 3)
		    return 0;	
	      case 20:	
		max_drop_count-=1; 
		if(chance <= 4)
		    return 0;
	      default: return 1;	
           } 
       }

    return 1;
    }
