#include<stdio.h>
#include<stdlib.h>

#include "minifat.h"

/*
 * checks whether we have a little endian architecture and whether
 * we can specify 32 bit integers.
 */
 
int main(int argc, char * argv[]){
	int i = 1, i_s, j ;
	long l = 1, l_s ;
	char * cp ;
	int_32bit test_int ;
	int res = 0 ;

	i_s = sizeof(int) ;
	printf("Type int is %d bytes, i.e. %d bits.\n", i_s, 8*i_s ) ;
	l_s = sizeof(long) ;
	printf("Type long is %lu bytes, i.e. %lu bits.\n", l_s, 8*l_s ) ;

	l = 0x0807060504030201 ;
	cp = (char *) &l ;
	printf("The number %#lx is stored as: ", l) ;
	for (j=0;j<l_s;j++) {
		printf("%d ",cp[j]) ;
	}
	printf("\n\n") ;

	i = 1 ;
	cp = (char *) &i ;
	if ( ((int)(*cp))==i ) {
		printf("**** this architecture is little endian;\n "
		"     your code should work on this architecture ****\n") ;
	} else {
		printf("**** this architecture is big endian;\n"
		"     your code does not have to work on this architecture ****\n") ;
		res = 1 ;
	}

	if ( sizeof(test_int) != 4 ) {
		printf("**** typdef int_32bit is not set correctly for this architecture ****\n"
		"Edit minifat.h and correct to past this test.\n") ;
		res =1 ;
	}
	else 
		printf("**** typdef int_32bit is set correctly ****\n") ;
	printf("\n\n") ;

	return res ;
}

