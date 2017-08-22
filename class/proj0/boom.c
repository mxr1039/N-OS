#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "boom.h"

/*
 * author: burt
 * date: 24 aug 2015
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change name and continue work
 *
 * update: 29 aug 2016
 */


/* globals go here */
int g_debug = 0 ;

/* defines go here, in SHOUTY_CASE */
#define N_DEFAULT 1
#define USAGE_MESSAGE "usage: boom [-vu] [-n stepsize] count" 

int main(int argc, char * argv[]) {

	int is_verbose = 0 ;
	int ch ;
	int n = N_DEFAULT ;

	while ((ch = getopt(argc, argv, "D:n:vu")) != -1) {
		switch(ch) {

		/*
                 * modify or add to these case statements
		 */

			case 'D':
				g_debug = atoi(optarg) ;
				break ;
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

	/* example of an assertion */
	assert(n>0) ;

	/* 
	 * your code here
	 *
	 */

	return 0 ;
}

