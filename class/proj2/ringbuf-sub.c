#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "ringbuf.h"
#include "mysyscalls.h"

/*
 * author: 
 * date: 
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change "author" and continue work
 * Hint: see mysyslog.c to see how to use syscall( syscall-number, arguments ... ) 
 *
 * update: 
 */

int g_debug = 0 ;
int is_verbose = 0 ;

int rb_ioctl(int op) {
	switch (op) {
	case RB_Q_SIZE:
	case RB_IS_EMPTY:
	case RB_IS_FULL:
	case RB_Q_COUNT:
	default:
		;
	}
	return 0 ;
}

int rb_enqueue(int ele) {
	return 0 ;
}

int rb_dequeue(void) {
	return -1 ;
}

