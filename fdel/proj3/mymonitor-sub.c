#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "mymonitor.h"
#include "mysyscalls.h"

/*
 * this is a template.
 * you are to complete the code for the subroutine.
 * hint: the actual code for the monitor is in the kernel.
 *
 * author: francisco delger
 * date: 
 * pledge: this is my own work, unless otherwise noted
 *
 * last update of template: 10 oct 2016
 *
 */

extern int is_verbose ;

int my_monitor(int op) {
    return syscall(MY_MONITOR, op);
}

