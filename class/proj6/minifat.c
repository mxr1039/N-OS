#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<assert.h>
#include<errno.h>

/*
#define USE_FUSE_VERSION must come before #include<fuse.h>
*/

#define FUSE_USE_VERSION 30
#include<fuse.h>

#include "minifat.h"
#include "fuseops.h"
#include "mf-utils.h"

/*
 * program to implement minifat 
 *
 * author: 
 * created: 12 nov 2016
 * last-update: 17 nov 2016
 */
 
int is_verbose = 0 ;
extern int errno ; 
void * vd_mm = NULL ; // base of virtual disk

static struct fuse_operations fuse_oper = {
	// call backs for fuse
	.getattr = fs_getattr,
	.readdir = fs_readdir,
	.mkdir   = fs_mkdir,
	.rmdir   = fs_rmdir,
	.create  = fs_create,
	.unlink  = fs_unlink,
	.utimens = fs_utimens,
	.truncate = fs_truncate,
	.open    = fs_open,
	.read    = fs_read,
	.write   = fs_write,
} ;

void print_usage(void) {
	printf("Usage: minifat -cv -d _directory_ _virtual_disk_name_\n") ;
	return ;
}
int main(int argc, char * argv[]) {

	int ch ;
	char * mount_dir = NULL ;
	int is_create = 0 ;
	
	assert(sizeof(int_32bit)==4) ;
	
	while ((ch = getopt(argc, argv, "vcd:")) != -1) {
		switch(ch) {
		case 'v':
			is_verbose = 1 ;
			break ;
		case 'c':
			is_create = 1 ;
			break ;
		case 'd':
			mount_dir = strdup(optarg) ;
			break ;
		default:
			print_usage() ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc<1 || !mount_dir) {
		print_usage() ;
		return 0 ;
	}
	
	if ( is_create ) {
		/* initialize the virtual disk */
	}
	assert( vd_mm ) ;

	if ( is_verbose ) {
		char * argv_fuse[3]  = { "", "-d", mount_dir, NULL } ;
		return fuse_main(3, argv_fuse, &fuse_oper, NULL) ;	
	}
	else {
		char * argv_fuse[2]  = { "", mount_dir, NULL } ;
		return fuse_main(2, argv_fuse, &fuse_oper, NULL) ;	
	}
	assert(0==1) ;
}
