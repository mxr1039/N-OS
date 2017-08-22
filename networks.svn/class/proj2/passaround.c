/*
** name: passaround.c
**
** author:
** date:
** last modified:
**
** from template created 31 jan 2015 by bjr
**
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>

#include "passaround.h"

#define LOCALHOST "localhost"
#define MAXMSGLEN 2048
#define N_REPEAT_DEFAULT 1

#define USAGE_MESSAGE "usage: passaround [-v] [-n num] [-m message] port"

int g_verbose = 0 ;

int main(int argc, char * argv[]) {
	int ch ;
	int the_port = 0 ;
	int loop = 1 ;
	int n_repeat = N_REPEAT_DEFAULT ;
	char * msg = NULL ;
	int is_forever = 0 ;
	
	assert(sizeof(short)==2) ; 
	
	while ((ch = getopt(argc, argv, "vm:n:")) != -1) {
		switch(ch) {
		case 'n':
			n_repeat = atoi(optarg) ;
			break ;
		case 'v':
			g_verbose = 1 ;
			break ;
		case 'm':
			msg = strdup(optarg) ;
			break ;
		case '?':
		default:
			printf(USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
    argv += optind;
    
    if ( argc!= 1 ) {
    	fprintf(stderr,"%s\n",USAGE_MESSAGE) ;
    	exit(0) ;
    }
    
    the_port = atoi(*argv) ;
    assert(the_port) ;

	is_forever = (n_repeat == 0) ;
	
	if ( msg ) {

		// parse and send
	
		free(msg) ;
		n_repeat-- ; // a packet sent
	}
	
    while( is_forever || n_repeat ) {
    	
    	// listen for a packet
    	// print R: host:port |message|

		// if something to send, {
    	//    parse and send
    	//    and print S: host:port |message|
    	// }
    
		n_repeat-- ;
	}
	return 0 ;
}

