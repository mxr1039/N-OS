#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "arrange-h-e.h"

/*
 * author: burt
 * date: 20 jan 2016
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change name and continue work
 */


/* globals go here */
int g_debug = 0 ;

/* defines go here, in SHOUTY_CASE */
#define USAGE_MESSAGE "usage: arrange-h-e [-v] arg" 
#define PARSE_CHAR ':'

struct Node * create_node(void) {
	// use malloc
	return NULL ;
}

void destroy_node(struct Node * n) {
	// use free 
	return ;
}

/*
 * implement
 */
 

int main(int argc, char * argv[]) {

	int is_verbose = 0 ;
	char * msg = NULL ;
	int ch ;
	
	// starter code for using getopt
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
			case 'v':
				is_verbose = 1 ;
				break ;
			default:
				printf("%s\n", USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	
	if ( argc==0 ) {
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
	}
	msg = argv[0] ;

	// an example of an assertion
	assert(msg) ;

	// an example of if is_verbose
	if ( is_verbose ) {
		printf("%s:%d: argument is |%s|\n", __FILE__, __LINE__, msg ) ;
	}
	
	/* 
	 * implement
	 *
	 */

	return 0 ;
}

