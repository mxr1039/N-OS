#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<sys/wait.h>

#include "pc-monitor.h"

/*
 * consumer template 
 * author:francisco delger
 * created:
 * lastupdate:
 */

#define USAGE_MESSAGE "usage: consumer [-vM] [-r _num_] [-n _num_]"
#define REPEAT_TIME_DEFAULT 1

int is_verbose = 0 ;

int main( int argc, char * argv[]) {
	int ch ;
	int repeat = REPEAT_TIME_DEFAULT ;
	int is_mm = 0 ;
	int num = 0;
	char s[32];
	int DequeueLetters = 0;
	
       
	while ((ch = getopt(argc, argv, "vMn:r:")) != -1) {
		switch(ch) {
		case 'r':
		        repeat = atoi(optarg);
			break ;
		case 'M':
		        is_mm = 1;
			break ;
		case 'v':
		        is_verbose = 1;
			break ;
		case 'n':
		        num = atoi(optarg);
			break;
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	memset(s,0,32);
	
        
	/*	while(repeat > 0) {
	consume(NULL,DequeueLetters);
	repeat--;
	}*/
	if (is_mm) {
	  if(is_verbose)
	    printf("We are in message mode \n");
	  if (num > 0) {
	    for(int j = 0; j < num; j ++) {
	      consume(s,32);

	    }

	  }

	} else {
	      if (is_verbose)
	        printf("We're doing single character\n");
	      if(num > 0) {
	  
	      for(int i = 0; i < num ; i++) {
	         consume(s, 1);
	         sleep(repeat);
	      }


	  }
	}
	return 0 ;
}
