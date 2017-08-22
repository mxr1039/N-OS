#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<ctype.h>
#include "boom.h"

/*
 * author: burt/francisco delger
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
        int count_up = 0;
        int count_to = 0;
	int counting_up = 0; // Second Variable for the counting up loop
        opterr = 0;
        int i; // for loop variable
	int j; // count up loop
   	while ((ch = getopt(argc, argv, "D:n:vu")) != -1) {
		switch(ch) {

		/*
                 * modify or add to these case statements
		 */
		        case 'n':
			        n = atoi(optarg);
				break; 
		        case 'u':
                                count_up = 1;
                                break; 
			case 'D':
				g_debug = atoi(optarg) ;
				break ;
			case 'v':
				is_verbose = 1 ;
				break ;
			case '?':
			        if(isdigit(optopt)) ;
			        return 0 ;
 		        default:
				printf("%s\n", USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	count_to = atoi(*argv) ;
	/* example of an assertion */
	assert(n>0) ;

	if (is_verbose) {
	  printf("Is verbose activated \n");
	  printf("The value of count up is %d \n" , count_up);
	  printf("The value of stepsize is %d \n", n);
          printf("We're counting up to %d \n", count_to);
	}

	counting_up = count_to;
	
	// Handle default, count down.
        if (!count_up) {

	  for ( i = count_to ; i > 0 ; i-- ) {
	    
	    printf("%d\n", count_to) ;
	    count_to = count_to - n ;
	  
	    if (count_to <= 0) {
	    
	        printf("%d\n", 0); // Doing this to avoid negative number
	        printf("Boom!\n");  
	        break;
		
	    } 

	  }
	  
	}
	
	if ( count_up == 1 ) {
	  for ( j = 0 ; j <= counting_up ; j = j + n ) {
	    printf ("%d\n", j) ;

	    if ( j + n > counting_up) {
      	      j = counting_up;
	    }
	      
	    if ( j == counting_up) {
	      
	      printf("Boom!\n");
	    }
	  }
	}
        
	return 0 ;
}

