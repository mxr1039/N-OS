/*
** name: ttftp.c
**
** author: bjr
** created: 31 jan 2015 by bjr
** last modified: 26/03/2016
**		14 feb 2016, for 162 semester of csc424 -bjr 
**
** from template created 31 jan 2015 by bjr
*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>
#include<unistd.h>

#include "ttftp.h"

#define USAGE_MESSAGE "usage: ttftp [-vL] [-h hostname -f filename -d timeout] port"

int g_verbose = 0 ;  // global declaration; extern definition in header 

int main(int argc, char * argv[]) {
	int ch ;
	int is_server = 0 ;
	int port = 0 ; 
	int is_noloop = 0 ; 
	char * hostname = NULL ;
	char * filename = NULL ;
        int timeout = 4; //default is 4 seconds
	// check whether we can use short as the data type for 2 byte int's
	assert(sizeof(short)==2) ;

	while ((ch = getopt(argc, argv, "vLf:h:")) != -1) {
		switch(ch) {
		case 'v':
			g_verbose ++ ;
			break ;
		case 'h':
			hostname = strdup(optarg) ;
			break ;
		case 'f':
			filename = strdup(optarg) ;
			break ;
		case 'L':
			is_noloop = 1 ;
			break ;
			//case 'd': I segfault if I try to enable this
		        //timeout = atoi(optarg);
                        //break;
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc!= 1 ) {
			fprintf(stderr,"%s\n",USAGE_MESSAGE) ;
		exit(0) ;
	}
	port = atoi(*argv) ;

	if (hostname == NULL && filename == NULL)
	  is_server = 1;

	if (!is_server ) {
		/* is client */
		return ttftp_client( hostname, port, filename ) ;
	}
	else {
		/* is server */
	  return ttftp_server( port, timeout, is_noloop ) ;
	}
	
	assert(1==0) ;
	return 0 ;
}

