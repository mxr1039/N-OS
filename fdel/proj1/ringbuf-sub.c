#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "ringbuf.h"

/*
 * author: 
 * date: 
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change "author" and continue work
 *
 * update: 
 */


/* static variables (zeroed) */

int g_debug = 0 ;
int is_verbose = 0 ;
struct RingBuf rb ;

int rb_ioctl(int op) {
	switch (op) {
	case RB_Q_SIZE:
		return RINGBUF_SIZE ;
	case RB_IS_EMPTY:
	  if(rb.is_full == 0 && rb.head == rb.tail)
	     return 1;
	  else
		return 0 ;
	case RB_IS_FULL:
	  if(rb.is_full == 1 )
	     return 1;
	  else
	    return 0;
	case RB_Q_COUNT:
	  if (rb.is_full == 1)
	    return 16;
	  return rb.head - rb.tail;
	default:
		;
	}
	return 0 ;
}

int rb_enqueue(int ele) {
       if (rb.is_full == 1 )
         return -1;
        rb.ringbuf[rb.head] = ele;
        rb.head++;
        if (rb.head == 16)
	  rb.head = rb.head % RINGBUF_SIZE;
	if (rb.head == rb.tail )
	  rb.is_full = 1;
	return 0 ;
}

int rb_dequeue(void) {
  int temp; 
  if (rb.head == rb.tail && rb.is_full == 0 )
	return -1 ;
  temp = rb.ringbuf[rb.tail];
  rb.tail++;
  rb.is_full = 0;
  return temp;
}

