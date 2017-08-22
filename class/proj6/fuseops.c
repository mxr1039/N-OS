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

/*
 * program to implement fuse ops 
 *
 * author: 
 * created: 17 nov 2016
 * last-update: 
 */
 
extern int is_verbose ;
extern int errno ; 

int fs_getattr(const char * path, struct stat * stbuf) {
	return 0 ;
}

int fs_readdir(const char * path, void * buf, 
	fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi ) {
	return 0 ;
}

int fs_mkdir( const char * path, mode_t mode ) {
	return 0 ;
}

int fs_rmdir( const char * path ) {
	return 0 ;
}

int fs_create( const char * path, mode_t m, struct fuse_file_info * fi ) {
	return 0 ;
}

int fs_unlink( const char * path ) {
	return 0 ;
}

// touch prefers this implemented 
int fs_utimens( const char * path, const struct timespec tv[2] ) {
	return 0 ;
}

int fs_truncate(const char * path, off_t offset) {
	return 0 ;
}
	
int fs_open(const char * path, struct fuse_file_info * fi) {
	return 0 ;
}

int fs_read(const char * path, char * buf, size_t size, off_t offset, 
	struct fuse_file_info * fi) {
	return 0 ;
}

int fs_write(const char * path, const char * buf, size_t size, off_t offset,
	struct fuse_file_info * fi) {
	return 0 ;
}

