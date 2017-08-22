#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<sys/wait.h>

#include "pc-monitor.h"

/*
 * producer template 
 * author:
 * created:
 * lastupdate:
 */

#define USAGE_MESSAGE "usage: producer [-vM] [-r _num_] [-n _num_] _message_"
#define REPEAT_TIME_DEFAULT 1

#define WORD_DELIMIT '.'

int is_verbose = 0 ;

int main( int argc, char * argv[]) {
	int ch ;
	int repeat = REPEAT_TIME_DEFAULT ;
	int is_mm = 0 ; 
	int num = 0 ; 
	char * s = NULL;
	char * token;
	char * delimiter = ".";
	int len = 0;
	/* usual getopt stuff */
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
			break ;
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	s = strdup(*argv);
	len = strlen(s);
	
	if (is_mm) {
	  token = strtok(s, delimiter);
	  len = strlen(token);
	  produce(token, len);
	  sleep(repeat);
	  while ( num > 0 ) {
	    
	    if (token == NULL && num > 0) {
	      token = strtok(s, delimiter);
	      len = strlen(token);
	      produce(token, len);
	      sleep(repeat);
	    }
	    
	    token = strtok(NULL, delimiter);
	    len = strlen(token);
	    
	    if (is_verbose) {
	      printf("token second round is %s\n", token);
	      printf("len second round is %d \n", len);
	    }
	    if ( token != NULL ) {
	      produce(token, len);
	      sleep(repeat);
     	    }
	    num--;

	  }
	}
	else {
	  while (num > 0) {
	   produce(s,len);
	   sleep(repeat);
	   num--;
	  }
	}
	
	return 0 ;
}
