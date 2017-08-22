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
	  return syscall(RB_IOCTL, 0);
	case RB_IS_EMPTY:
	  return syscall(RB_IOCTL, 1);
	case RB_IS_FULL:
	  return syscall(RB_IOCTL, 2);
	case RB_Q_COUNT:
	  return syscall(RB_IOCTL, 3);
	default:
		;
	}
	return 0 ;
}

int rb_enqueue(int ele) {
  return syscall(RB_ENQUEUE, ele) ;
}

int rb_dequeue(void) {
  return syscall(RB_DEQUEUE) ;
}

