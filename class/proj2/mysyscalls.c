#include<linux/kernel.h>
#include<linux/syscalls.h>

/*
 * kernel/mysyscalls.c
 *
 * this is a template of the file that is in the kernel and implements
 * the four new system calls: 
 *     my_syslog, rb_ioctl, rb_enqueue, and rb_dequeue
 *
 * author:
 * date:
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
	return 0 ;
}

asmlinkage long sys_rb_enqueue(char c) {
	return 0 ;
}

asmlinkage long sys_rb_dequeue(void) {
	return 0 ;
}

