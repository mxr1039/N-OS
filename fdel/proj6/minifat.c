
#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>


#include "minifat.h"
//#include "dos_filenames.c"

/*
 author: bjr
 created: 4 dec 2015
 last update:

 hint file for getting started on fatfuse
 
 */


/*
  parts of this file are from hello.c, fuse.sourceforge.net/doxygen/files.html
  and have the notice:
  
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

*/

#define USAGE_MESSAGE "minifat -d _mount_ [-cv] _virtualdisk_"
#define LOGFILE_NAME "logfile"
#define EIGHT_PLUS_THREE 11


static struct {
	void * vd_mm ;
	FILE * logfile ;
	int is_verbose ;
} g ;


//char * dos_filename(char * name) {
/* 
 * converts string to 8+3 filename in dirent format.
 * returns static buffer, or NULL if illegal filename
 * beware: 
 *   does not check for illegal characters in dos filename
 *   does not check for illegal character in unix filename
 *   does not convert to uppercase
 *//*
	static char dos_name[11] ;
	char * s ;
	unsigned int i ;

	memset(dos_name, ' ', 11) ;
	s = strchr(name, '.') ;
	if (!s) {
		s = name + strlen(name) ; 
	}
	/* ASSERT *s == '.' || *s == '\0' */
//	i = s-name ;
//	if (i>8) // return NULL ;
//		i = 8 ;
//	memcpy(dos_name, name, i ) ;
//	if ( *s=='.' ) {
//		i = strlen(s+1) ;
//		if ( i>3 ) // return NULL ;
//			i = 3 ;
//		memcpy(dos_name+8, s+1, i) ;	
//	}
//	return dos_name ;
//}


//char * unix_filename(char * name) {
/*
 * converts a 11 character buffer of a filename in
 * the format of a FAT directory entry to a string
 *
 * returns a pointer to a static buffer
 */

//	static char ux_name[13] ;
//	int i = 0 ;
//	int j ;
//	memset(ux_name, '\0', 13) ;
//	while ( name[i]!=' ' && i<8 ) {
//		ux_name[i] = name[i] ;
//		i++ ;
//	}
//	j = i ;
//	i = 8 ;
//	if (/ name[i] != ' ' ) {
	  //		ux_name[j++] = '.' ;
//		while ( name[i]!=' ' && i<11 ) {
//			ux_name[j++] = name[i++] ;
//		}
//	}
//	return ux_name ;
//}


struct Dirent * find_file_dent(char * dos_name, int clust) {
	// given an 11 byte array with dos_name, and a cluster number,
	// find the file with name dos_name in the one cluster directory in cluster clust
	// return NULL if not found, or a pointer to the found Dirent.
	
	char * b ;
	struct Dirent * dent ;
        struct Dirent * this_dent;
	int_32bit next;
	struct Fat * fat;
	
	b = ((char *) g.vd_mm) + clust * CLUSTER_SIZE ;
	dent = (struct Dirent *) b  ;

	for(this_dent = dent; this_dent < dent + 32; this_dent++) {
	  if(memcmp(dos_name, this_dent->dir_name, EIGHT_PLUS_THREE) == 0) {
	    return this_dent;
	  }

	}
	// a for loop here to walk cluster, looking (use memcmp)
	fat = (struct Fat *)(((char *)g.vd_mm) + FAT_TABLE_CLUSTER * CLUSTER_SIZE);
			     
	for (int i = 5; i < CLUSTER_NUMBER; i++) {
	  if(fat->table[i] == -1) {
	   fat->table[i] == 0;
	   break;
	 }
	}
	if(next != -1) {
	  return find_file_dent(dos_name, next);

	}
	return NULL ;
}


struct Dirent * find_free_dent(int clust) {
	// find a free Dirent in the one cluster directory in cluster clust
	// return a pointer to the free Dirent, or NULL if none found.
	
	char * b ;
	struct Dirent * dent ;
	struct Dirent * this_dent;
	int_32bit previous;
	int_32bit next;
	int_32bit tail;
	struct Fat * fat;

	next = previous = clust;
	fat = (struct Fat *)(((char *)g.vd_mm) + FAT_TABLE_CLUSTER * CLUSTER_SIZE);
	while ( next != -1) {
	  
	  b = ((char *) g.vd_mm) + clust * CLUSTER_SIZE ;
	  dent = (struct Dirent *) b  ;

	  for(this_dent = dent; this_dent < dent + 32; this_dent++ ) {
	    if ( this_dent->dir_name[0] == '\0' ) {
	      return this_dent;
	    }
	  }
	  previous = next;
	  for (int i = 5; i < CLUSTER_NUMBER; i++) {
	    if(fat->table[i] ==-1) {
	      fat->table[i] = 0;
              next = i;
	    }
	  }
	 
	}
			     //	tail = find
	return NULL ;
}


static int mf_getattr(const char *path, struct stat *stbuf) {

	if (g.is_verbose) {
		fprintf(g.logfile,"getattr: %s\n", path ) ;
		fflush(g.logfile) ;
	}

	memset(stbuf, 0, sizeof(struct stat));

	struct Dirent * dirent;
	int res;

	if(strcmp(path, "/") == 0) {
	  
	  stbuf->st_mode = S_IFDIR | 0755;
	  stbuf->st_nlink = 2;
	  stbuf->st_size = 1024;
	  
	} else {
	  path = dos_filename(path);
	  dirent = find_file_dent(path, ROOT_DIR_CLUSTER);
	}

	if (dirent != NULL) {
	  if (dirent->dir_attr = 0x0) {
	    stbuf->st_mode = S_IFREG| 0777;
	    stbuf->st_nlink = 1;
	    stbuf->st_size = 0;
	  } else if(dirent->dir_attr = 0x10) {
	    stbuf->st_mode = S_IFDIR | 0777;
	    stbuf->st_nlink = 1;
	    stbuf->st_size = 1024;
	  }
	} else {
	  res = -ENOENT;
	}
	
	return res;
  
}


static int mf_readdir(const char *path, void *buf, 
	fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {

        (void) offset;
        (void) fi;
	struct Dirent * dent ;
	char * b ;
        struct Dirent * i_dent;

        if(g.is_verbose) {
          fprintf(g.logfile, "readdir: %s\n", path);
          fflush(g.logfile);
         }

        if (strcmp(path, "/") != 0)
           return -ENOENT;

        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        b = ((char*) g.vd_mm) + ROOT_DIR_CLUSTER * CLUSTER_SIZE;
        dent = (struct Dirent *) b;

        for(i_dent = dent; i_dent < dent + 32; i_dent++) {
          if(i_dent->dir_name != '\0') {
	    strcpy(i_dent->dir_name, unix_filename(i_dent->dir_name));
	    filler(buf, i_dent->dir_name, NULL, 0);
	  }
	}

	return 0 ;	
}

static int mf_mkdir( const char * path, mode_t mode ) {
  struct Fat * fat;
  struct Dirent * dirent;
  int j;
  path = dos_filename(path);
  dirent = find_file_dent(path, ROOT_DIR_CLUSTER);
  fat = (struct Fat *)(((char *)g.vd_mm) + FAT_TABLE_CLUSTER * CLUSTER_SIZE);
  if ( dirent == NULL)
    return 0;
  else {
    dirent = find_free_dent(ROOT_DIR_CLUSTER);
    strcpy(dirent->dir_name, path);
    for( int i = 5; i < CLUSTER_NUMBER; i++) {
      if(fat->table[i] == -1) {
	fat->table[i] = 0;
	j = i;
	break;
      }
      if( i == CLUSTER_NUMBER - 1)
	return -1;
    }
  }
  dirent->dir_firstcluster = j;
  mode = S_IFDIR | 0777;
  dirent->dir_attr = 0x10;
  return 0;
}

static int mf_rmdir( const char * path ) {
  struct Dirent * dirent;
  struct Dirent * dent;
  struct Dirent * i_dent;
  path = dos_filename(path);
  dirent = find_file_dent(path, ROOT_DIR_CLUSTER);
  if(dirent->dir_attr != 0x10)
    return -ENOTDIR;
  if(dirent != NULL) {
    dent = (struct Dirent*)(((char *)g.vd_mm) + dirent->dir_firstcluster * CLUSTER_SIZE);
    for( i_dent = dent; i_dent < dent + 32; i_dent++) {
      if(i_dent->dir_name != '\0') {
	strcpy(i_dent->dir_name, '\0');
	return 0;
      }
    }
  }
  return 0;
}


static int mf_open(const char *path, struct fuse_file_info *fi) {

	if (g.is_verbose) {
		fprintf(g.logfile,"open: %s\n", path ) ;
		fflush(g.logfile) ;
	}
	
	// find file, and return -ENOENT or 0
	
	return 0 ;
}


static int mf_unlink(const char * path ) {
	struct Dirent * dent ;
	char * dos_name ;
	struct Dirent * dirent;
	struct Dirent * i_dent;
	if(g.is_verbose) {
	  fprintf(g.logfile,"open: %s\n", path) ;
	  fflush(g.logfile) ;
	}
	
	path = dos_filename(path);
	dirent = find_file_dent(path, ROOT_DIR_CLUSTER);
	if(dirent->dir_attr == 0x10)
	  return -EISDIR;
	else if(dirent->dir_attr == 0x0 && dirent != NULL) {
	  dent = (struct Dirent *)(((char *)g.vd_mm) + dirent->dir_firstcluster * CLUSTER_SIZE);
	  for (i_dent = dent; i_dent < dent + 32; i_dent++) {
	    if(i_dent->dir_name != '\0' && i_dent->dir_attr == 0x0) {
	      i_dent->dir_name == '\0';
	      return 0;
	    }
	  }
	} else {
	  return -ENOENT;
	}
	// find file, and if found, zero out the Dirent	
	return 0 ;
}


static int mf_create(const char * path, mode_t mode, 
	struct fuse_file_info * ffi ) {
	char * dos_name ;
	struct Dirent * dent ;
	struct Fat * fat;
	struct Dirent * dirent;
	int j;
	path = dos_filename(path);
	if (g.is_verbose) {
		fprintf(g.logfile,"create: %s\n", path ) ;
		fflush(g.logfile) ;
	}
	dirent = find_file_dent(path, ROOT_DIR_CLUSTER);
	fat = (struct Fat *)(((char*)g.vd_mm) + FAT_TABLE_CLUSTER * CLUSTER_SIZE);
	if ( dirent == NULL )
	  return 0;
	else {
	  dirent = find_free_dent(ROOT_DIR_CLUSTER);
	  strcpy(dirent->dir_name, path);
	  for ( int i = 5; i < CLUSTER_NUMBER; i++) {
	    if(fat->table[i] == -1) {
	      fat->table[i]=0;
	      j = i;
	      break;
	    }
	    if(i == CLUSTER_NUMBER - 1)
	      return -1;
	  }
	  dirent->dir_firstcluster = j;
	  mode = S_IFREG | 0777;
	  dirent->dir_attr = 0x0;
	}	
	return 0 ;
}


static int mf_read(const char *path, char *buf, size_t size, off_t offset,
	struct fuse_file_info *fi) {

	if (g.is_verbose) {
		fprintf(g.logfile,"read: %s\n", path ) ;
		fflush(g.logfile) ;
	}

	return 0 ;
}


static int mf_truncate(const char * path, off_t o_t ) {
	if (g.is_verbose) {
		fprintf(g.logfile,"truncate: %s\n", path ) ;
		fflush(g.logfile) ;
	}
	return 0 ;
}


static int mf_utimens(const char * path, const struct timespec t[2]) {
	if (g.is_verbose) {
		fprintf(g.logfile,"utimens: %s\n", path ) ;
		fflush(g.logfile) ;
	}
	return 0 ;
}


static int mf_write(const char * path, const char * buf, size_t s_t,
	off_t o_t, struct fuse_file_info * ffi ) {
	if (g.is_verbose) {
		fprintf(g.logfile,"write: %s\n", path ) ;
		fflush(g.logfile) ;
	}
	return 0 ;
}


static struct fuse_operations mf_oper = {
	.getattr	= mf_getattr,
	.readdir	= mf_readdir,
	.mkdir          = mf_mkdir,
	.rmdir          = mf_rmdir,
	.open		= mf_open,
	.read		= mf_read,
	.unlink		= mf_unlink,
	.create		= mf_create,
	.truncate	= mf_truncate,
	.utimens	= mf_utimens,
	.write		= mf_write,
};


void initialize_vd(int vd_id, void * g_vd_mm) {
	struct Fat * v_fat ;
	int i ;
	char * b ;
	struct Dirent * dent ;
	
	/* clear FAT table */
	v_fat = (struct Fat *) (((char *) g_vd_mm) + FAT_TABLE_CLUSTER * CLUSTER_SIZE) ;
	for (i=0; i<CLUSTER_NUMBER; i++ ) {
		v_fat->table[i] = 0 ;
	}
	
	/* claim block ROOT_DIR_CLUSTER in fat table */
	v_fat->table[ROOT_DIR_CLUSTER] = (int_32bit) -1 ;
	
	/* clear root directory */
	b = ((char *) g_vd_mm) + ROOT_DIR_CLUSTER * CLUSTER_SIZE ;
	for (i=0; i<CLUSTER_SIZE; i++) b[i] = (char) 0 ;
	
	/* make first directory entry */
	// directories no longer have parent pointers, nov 2015
	dent = (struct Dirent *) b ;
	for (i=0; i<sizeof(dent->dir_name); i++) {
	  dent->dir_name[i] = DIRENT_PARENT_NAME[i] ;
	}
	dent->dir_attr = DIRENT_ATTR_DIRECTORY ;
	dent->dir_firstcluster = (int_32bit) ROOT_DIR_CLUSTER ;

	return ;
}


int main(int argc, char *argv[])
{
	int ch ;
	int is_create = 0 ;
	char * v_disk_name ;
	int vd_id ;
	char * lf_name = LOGFILE_NAME ;

	
	char * argv_x[2] ;
	
	argv_x[0] = argv[0] ;
	argv_x[1] = NULL ;
	
	while ((ch = getopt(argc, argv, "vhcd:")) != -1) {
		switch(ch) {
		case 'v':
			g.is_verbose = 1 ;
			break ;
		case 'c':
			is_create = 1 ;
			break ;
		case 'd':
			argv_x[1] = optarg ;
			break ;
		case 'h':
		default:
			printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc!=1 ) {
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
	}
	
	if ( !argv_x[1] ) {
		printf("-d option is not optional\n") ;
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;	
	}

	v_disk_name  = argv[0] ;

	if ( g.is_verbose ) {
		g.logfile = fopen(lf_name,"w") ;
	}

	if ( g.is_verbose ) {
		fprintf(g.logfile, "mount: %s\ndiskfile: %s\nlogfile: %s\n", 
			argv_x[1], v_disk_name, lf_name ) ;
		fflush(g.logfile) ;
	}

	vd_id = open(v_disk_name, O_RDWR | O_CREAT, 0777) ;
	if ( vd_id==-1 ) {
		printf("Error: failed to open file %s, exiting.\n", v_disk_name) ;
		return 0 ;
	}
	ftruncate(vd_id, CLUSTER_NUMBER * CLUSTER_SIZE ) ;
	
	g.vd_mm = mmap(NULL, CLUSTER_NUMBER * CLUSTER_SIZE, 
		PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, vd_id, 0 ) ;
	if ( g.vd_mm == MAP_FAILED ) {
		perror("Error in mmap") ;
		return 0 ;
	}
	if ( is_create ) initialize_vd(vd_id, g.vd_mm) ;

	return fuse_main(2, argv_x, &mf_oper, NULL);

}

// end of file

