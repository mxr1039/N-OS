#include<linux/kernel.h>
#include<linux/syscalls.h>

/*
 * kernel/mysyscalls.c
 *
 * this is a template.
 * you are to complete the coding of the subroutines.
 * note: proj2 completed the coding of the ring buffer
 * subroutines; bring forward that code. 
 * for proj3 complete the coding of the monitor subroutine.
 *
 * author:
 * date:
 *
 * last update of template: 10 oct 2016
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

asmlinkage long sys_my_monitor(unsigned int op) {
	return 0 ;
}

