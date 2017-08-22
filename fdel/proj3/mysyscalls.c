#include<linux/kernel.h>
#include<linux/syscalls.h>

/*
 * kernel/mysyscalls.c
 *
 * this is a template of the file that is in the kernel and implements
 * the four new system calls: 
 *     my_syslog, rb_ioctl, rb_enqueue, and rb_dequeue
 *
 * author: francisco delger
 * date 10-20-2016:
 *
 */

#include "mysyscalls.h"

#define RINGBUF_SIZE 16

struct RingBuf {
	int head ;
	int tail ;
	int is_full ;
	char ringbuf[RINGBUF_SIZE] ;
} ;

struct RingBuf rb ;


asmlinkage long sys_my_syslog(const char * msg) {
	printk(KERN_ALERT "my_syslog: %s\n", msg) ;
	return 0 ;
}

asmlinkage long sys_rb_ioctl(unsigned int op) {

 switch (op) {
     case 0:
      return RINGBUF_SIZE;
     case 1:
       if(rb.is_full == 0 && rb.head == rb.tail)
         return 1;
       else
         return 0;
     case 2:
       if(rb.is_full == 1)
         return 1;
        else
         return 0;
     case 3:
       if(rb.is_full == 1)
         return RINGBUF_SIZE;
       if(rb.head > rb.tail)
         return rb.head - rb.tail;
       if(rb.head == rb.tail && rb.is_full == 0)
         return 0;
       else
         return RINGBUF_SIZE - rb.tail + rb.head;
     default:
             ;
 }
 return 0;
}

asmlinkage long sys_rb_enqueue(char c) {
  if (rb.is_full == 1)
    return -1;
  rb.ringbuf[rb.head] = c;
  rb.head++;
  if (rb.head == RINGBUF_SIZE)
    rb.head = rb.head % RINGBUF_SIZE;
  if (rb.head == rb.tail)
    rb.is_full = 1;
   return 0;
}

asmlinkage long sys_rb_dequeue(void) {
  int temp;
  if (rb.head == rb.tail && rb.is_full == 0)
    return -1;
  temp = rb.ringbuf[rb.tail];
  rb.tail++;
  if (rb.tail == RINGBUF_SIZE)
    rb.tail = rb.tail % RINGBUF_SIZE;
  rb.is_full = 0;
  return temp;
}

asmlinkage long sys_my_monitor(unsigned int op) {
  return 0;
}
