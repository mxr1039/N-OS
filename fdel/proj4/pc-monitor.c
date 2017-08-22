#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<sys/wait.h>
  
#include "mysyscalls.h"
#include "pc-monitor.h"

extern int is_verbose ;
int size;
int sizeC;
int discard;
int discardC;
int is_full = 0;
int is_empty = 0;
int cIndex = 0;
int pIndex = 0;


/*

DESCRIPTION

    Subroutines that are two entry points into a producer-consumer monitor.
    
    Produce enqueues on the ring buffer the sequence of len characters, taken from 
    the char array pointed to by s. 
    
    Consume dequeues from the ring buffer as sequence of up to len characters, and stores
    them in a char array pointed to by s.
    
    The character sequence need not be a string. The null character has no particular
    significance. 
    
    The ring buffer is queried for size. If len is larger than the size of the ring 
    buffer the additional produced characters are ignored, and the additional 
    consumed characters are discarded.
    
    Each call to produce contributes an atomic message to the ring buffer. All characters
    are enqueued together in the ring buffer, and a call to consume takes all characters
    from that message. That is, multiple consumers and producers working simultaneously
    do not confuse messages. 
    
    The procedures are thread-safe: each call enters and leaves the monitor as needed.
    Produce will wait until there is room in the ring buffer to enqueue the message;
    consume will wait until there is a message in the ring buffer to dequeue.

RETURN VALUE

    Returns the number of characters actually enqueued or dequeued, or -1 if error.


*/
int temp;
int temp2;

int consume(char * s, int len) {
  
  //  	assert(s) ; why assert when the calls to consumer.c dont even include a // message
      	
	syscall(MY_MONITOR,1); // enter

	if(is_verbose)
	  printf("Entered consumer monitor... \n");
	
	if ( len < 0 ) {

	  if(is_verbose)
	    printf("bad len consumer \n");

	  syscall(MY_MONITOR, 5); //notifyall
	  syscall(MY_MONITOR, 2); //leave
	  return -1;
	}
	
        if(is_verbose)
	  printf("I'm checking if len greater than size\n");
	sizeC = get_ringbuf_size();
	
	if ( len > sizeC) {
	  discardC = len - sizeC;
	  len = len - discardC;
	  if ( is_verbose)
	    printf("Discarded elements, too large...\n");

	}

	//is_empty = syscall(RB_IOCTL, 1);

       while (syscall(RB_IOCTL, 1)) {
	 if(is_verbose)
	   printf("I'm waiting because the consumer is empty...\n");
	 syscall(MY_MONITOR, 3);
       }
       
      while ( len > 0) {

	if ( (is_empty = syscall(RB_IOCTL, 1)) == 0) {	 
	 s[cIndex] = syscall(RB_DEQUEUE);
	} else {
	  break;
	}
	 
	 if (is_verbose) { 
	    printf("Dequeuing: %c \n", s[cIndex]);
	  }
	 
	  cIndex++;
	  len--;
	  
       }
       
	if(is_verbose)
	  printf("Completed consuming \n");
	  
	syscall(MY_MONITOR, 5);
	syscall(MY_MONITOR, 2);
	
	temp = cIndex;
	cIndex = 0;
	return temp ;	
}

int produce(char * s, int len) { 
  	assert(s) ;
	
	syscall(MY_MONITOR, 1);
	if(is_verbose)
	  printf("I entered the monitor produce side\n");
	
	if(len < 0) {
	  
	  if(is_verbose)
	    printf("bad len producer\n");

	  syscall(MY_MONITOR, 5);
	  syscall(MY_MONITOR, 2);

	  return -1;
	  
	}
 	
		
	while ( len > 0) {
          
	  if(is_verbose)
	    printf("I'm checking for size in producer..\n");
	  
	  size = get_ringbuf_size();
	  
	  if ( len > size ) {
	    discard = len - size;
	    len = len - discard;
	  }	  
          if (is_verbose)
	    printf("Checking if it's full... \n");
	  
	  is_full = syscall(RB_IOCTL, 2);
	  
	  if (is_full == 1) {
	    if (is_verbose)
	      printf("Full and waiting...\n");
	    syscall(MY_MONITOR, 3);
	  }

	  
	  syscall(RB_ENQUEUE, s[pIndex]);

	  if(is_verbose)
	    printf("enqueuing: %c\n", s[pIndex]);

	  pIndex++;
	  len--;

	}

	syscall(MY_MONITOR, 5);
	syscall(MY_MONITOR, 2);

	if(is_verbose)
	printf("Completed producing\n");
	temp2= pIndex;
	pIndex = 0;
	return temp2 ; 
} 

int get_ringbuf_size() {
  return syscall(RB_IOCTL, 0) ;
}

