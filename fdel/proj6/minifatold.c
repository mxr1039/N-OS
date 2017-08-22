
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
#include "dos_filenames.h"

/*
 * program to implement minifat 
 *
 * author: 
 * created: 12 nov 2016
 * last-update: 
 */
 
int is_verbose = 0 ;
extern int errno ;

static struct {
  void * vd_mm;
  FILE * logfile;
  //int is_verbose;
} g;

static int fs_getattr(const char * path, struct stat * stbuf) {
  struct Dirent dirent;
  int res;
  memset(stbuf,0, sizeof(struct stat));
  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    stbuf->st_size = 1024;
  } else {
    path = dos_filenames(path);
    dirent = find_file_dent(path, ROOT_DIR_CLUSTER);
  }

  if (dirent != NULL) {    
      if (dirent->dir_attr = 0x0) {
	stbuf->st_mode = S_IFREG | 0777;
	stbuf->st_nlink = 1;
	stbuf->st_size = 0;
      } else if { dirent->dir_attr = 0x10) {
	stbuf->st_mode = S_IFDIR | 0777;
	stbuf->st_nlink = 1;
	stbuf->st_size = 1024;
      }
  } else {
	res = -ENOENT; 
  }
	return res ;
}

static int fs_readdir(const char * path, void * buf, fuse_fill_dir_t filler,
		      off_t offset, struct fuse_file_info *fi) {
  (void) offset;
  (void) fi;
  struct Dirent * dent;
  char * b;
  struct Dirent i_dent;
  if (strcmp(path, "/") != 0)
    return -ENOENT;
  filler(buf, ".", NULL, 0);
  filler(buf. "..", NULL, 0);
  b = ((char*) g.vd_mm) ROOT_DIR_CLUSTER * CLUSTER_SIZE;
  dent = (struct Dirent *) b;
  for (i_dent = dent; i_dent < dent + 32 ; i_dent++) {
    if(i_dent->dir_name != '\0') {
      i_dent->dir_name = unix_filename(i_dent->dir_name);
      filler(buf, i_dent.dir_name, NULL, 0);
    }
  }   
        return 0 ;
}

static int fs_mkdir( const char * path, mode_t mode ) {
  struct Fat fat;
  struct Dirent * dirent;
  int j;
  path = dos_filename(path);
  dirent = find_file_dent(path);
  if (dirent == NULL)
	return 0 ;
  else {
    dirent = find_free_dent(ROOT_DIR_CLUSTER);
    dirent->dir_name = path;
    for (int i = 5; i < CLUSTER_NUMBER; i++) {
      if(fat->table[i] == -1) {
	fat->table[i] = 0;
	j = i; // if i return here the index then i cant reach the last lines
	break;
      }
      if(i == CLUSTER_NUMBER - 1)
	return -1;
    }
  }
  dirent->dir_firstcluster = j;
  mode = S_IFDIR | 0777;
  dirent->dir_attr = 0x10;
  return 0;
}

static int fs_rmdir( const char * path ) {
  struct Dirent * dirent;
  struct Dirent * dent;
  struct Dirent * i_dent;
  path = dos_filename(path);
  dirent = find_file_dent(path);
  if(dirent->dir_attr != 0x10)
    return -ENOTDIR;
  if (dirent != NULL) {
    dent = (struct Dirent *)(((char *) g.vd_mm) + dirent->dir_firstcluster * CLUSTER_SIZE);
    for ( i_dent = dent; i_dent < dent + 32; i_dent++) {
      if(i_dent->dir_name != '\0') {
        i_dent->dir_name = '\0';
        return 0;
      }
    }
  
  }
	return 0 ;
  
}

static int fs_create( const char * path, mode_t m, struct fuse_file_info * fi ) {
  struct Fat fat;
  struct Dirent * dirent;
  int j;
  path = dos_filename(path);
  dirent = find_file_dent(path);
  if (dirent == NULL)
     return 0 ;
  else {
    dirent = find_free_dent(ROOT_DIR_CLUSTER);
    dirent->dir_name=path;
    for(int i = 5; i < CLUSTER_NUMBER; i++) {
      if(fat->table[i] == -1) {
	fat->table[i] == 0;
	j = i;
	break;
      }
      if(i == CLUSTER_NUMBER - 1) 
	return -1;
    }
    dirent->dir_fircluster = j;
    mode = S_IFREG | 0777;
    dirent->dir_attr = 0x0;
  }
  return 0;
}

static int fs_unlink( const char * path ) {
  struct Dirent * dirent;
  struct Dirent * dent;
  struct Dirent * i_dent;
  path = dos_filename(path);
  dirent = find_file_dent(path);
  if(dirent->dir_attr == 0x10)
    return -EISDIR;
  else if (dirent->dir_attr == 0x0 && dirent !=NULL) {
    dent = (struct Dirent *)(((char *)g.vd_mm) + dirent->dir_firstcluster * CLUSTER_SIZE);
    for(i_dent = dent; i_dent < dent + 32; i_dent++) {
      if(i_dent->dir_name != '\0' && i_dent->dir_attr == 0x0) {
	i_dent->dir_name == '\0';
	return 0;
      }
    }
  } else {
    return -ENOTENT;
  }
	return 0 ;
}

// touch prefers this implemented 
static int fs_utimens( const char * path, const struct timespec tv[2] ) {
	return 0 ;
}

static struct fuse_operations fuse_oper = {
	// call backs for fuse
	.getattr = fs_getattr,
	.readdir = fs_readdir,
	.mkdir   = fs_mkdir,
	.rmdir   = fs_rmdir,
	.create  = fs_create,
	.unlink  = fs_unlink,
	.utimens = fs_utimens,
} ;

void print_usage(void) {
	printf("Usage: minifat -cv -d _directory_ _virtual_disk_name_\n") ;
	return ;
}
int main(int argc, char * argv[]) {

	int ch ;
	char * mount_dir = NULL ;
	char * v_disk_name;
	int vd_id;
	int is_create;
	
	
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
	/*
	v_disk_name = argv[0];
	vd_id = open(v_disk_name, O_RDWR | O_CREAT, 0777);
	g.vd_mm = mmap(NULL, CLUSTER_NUMBER * CLUSTER_SIZE, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, vd_id, 0);
	*/
	if ( is_create ) {
		/* initialize the virtual disk */
	  //initialize_vd(vd_id, g.vd_mm);
	}

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
