/*
** name: ttftp-enc.c
**
** author: bjr
** created: mar 2016 by bjr
** last modified:
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

#include "ttftp-enc.h"
#include "aes.h"

#define USAGE_MESSAGE "usage: ttftp-enc [-vL] [-h hostname -f filename] [-k key]  port"

int g_verbose = 0 ;  // global declaration; extern definition in header 

int main(int argc, char * argv[]) {
	int ch ;
	int is_server = 0 ;
	int port = 0 ; 
	int is_noloop = 0 ; 
	char * hostname = NULL ;
	char * filename = NULL ;
	char * key = NULL ;

	// check whether we can use short as the data type for 2 byte int's
	assert(sizeof(short)==2) ;

	while ((ch = getopt(argc, argv, "vLf:h:k:")) != -1) {
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
		case 'k':
			key = strdup(optarg) ;
			break ;
		case 'L':
			is_noloop = 1 ;
			break ;
		case '?':
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

	// sanity check inputs

	/* your code */

	if (!is_server ) {
		/* is client */
		return ttftp_client( hostname, port, key, filename ) ;
	}
	else {
		/* is server */
		return ttftp_server( port, key, is_noloop ) ;
	}
	
	assert(1==0) ;
	return 0 ;
}

