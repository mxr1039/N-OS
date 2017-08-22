#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<assert.h>

#include "minifat.h"

/*
 * program to format a minifat file system on a virtual disk
 * example of minifat.h struct usage as well as memory mapped files
 *
 * author: bjr
 * created: november 2014
 * last-upate: 7 nov 2016
 */
 
int is_verbose = 0 ;
extern int errno ; 

void print_usage(void) {
	printf("Usage: mmap-mf _virtual_disk_name_\n"
	  "  virtual disk should already exist, and be exactly 1M in size\n"
	  ) ;
	return ;
}

int main(int argc, char * argv[]) {

	int ch ;
	char * v_disk_name ;
	int vd_id ;
	void * vd_mm ;
	int i ;
	
	struct Fat * v_fat ;
	struct Dirent dent ; 
	char * b ;
	
	assert(sizeof(int_32bit)==4) ;
	
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
		case 'v':
			is_verbose = 1 ;
			break ;
		case '?':
		default:
			printf("usage: -d n for debug level") ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc<1 ) {
		print_usage() ;
		return 0 ;
	}
	v_disk_name = argv[0] ;    
	if ( is_verbose) printf("virtual disk file to open: %s\n", v_disk_name) ;
	
	vd_id = open(v_disk_name, O_RDWR) ;
	if ( vd_id==-1 ) {
		printf("Error: failed to open file %s, exiting.\n", v_disk_name) ;
		return 0 ;
	}
	vd_mm = mmap(NULL, CLUSTER_NUMBER * CLUSTER_SIZE, PROT_READ | PROT_WRITE, 
		MAP_FILE | MAP_SHARED, vd_id, 0 ) ;
	if ( vd_mm == MAP_FAILED ) {
		perror("Error in mmap") ;
		return 0 ;
	}
	
	/* clear FAT table */
	v_fat = (struct Fat *) (((char *)vd_mm) + FAT_TABLE_CLUSTER * CLUSTER_SIZE) ;
	for (i=0; i<CLUSTER_NUMBER; i++ ) {
		v_fat->table[i] = 0 ;
	}
	
	/* claim block ROOT_DIR_CLUSTER in fat table */
	v_fat->table[ROOT_DIR_CLUSTER] = (int_32bit) -1 ;
	
	/* clear root directory */
	b = ((char *)vd_mm) + ROOT_DIR_CLUSTER * CLUSTER_SIZE ;
	for (i=0; i<CLUSTER_SIZE; i++) b[i] = (char) 0 ;
	
	/* write a directory entry, example */
	memcpy(dent.dir_name,"hello   txt",11) ;
	dent.dir_attr = DIRENT_ATTR_READ_ONLY | DIRENT_ATTR_DIRECTORY ;
	dent.dir_firstcluster = 17 ;
	dent.dir_filesize = 20 ;
	
	memcpy(
		((char *)vd_mm) + ROOT_DIR_CLUSTER * CLUSTER_SIZE + 2 * sizeof(struct Dirent),
		&dent, sizeof(struct Dirent)) ;
		
	return 0 ;
}
