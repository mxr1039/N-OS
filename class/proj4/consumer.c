#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<sys/wait.h>

#include "pc-monitor.h"

/*
 * consumer template 
 * author:
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
	
	/* usual getopt stuff */
	while ((ch = getopt(argc, argv, "vMn:r:")) != -1) {
		switch(ch) {
		case 'r':
			break ;
		case 'M':
			break ;
		case 'v':
			break ;
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	consume(NULL,0);
	
	return 0 ;
}
