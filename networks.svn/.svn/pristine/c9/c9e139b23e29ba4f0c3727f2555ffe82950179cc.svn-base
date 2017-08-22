/*
 *  s_server.c
 *
 *  demo server using SSL/BIO. forked child listens and echos on a port
 *
 *  Created by Burton Rosenberg on 3/31/11.
 *  Copyright 2011 Burton Rosenberg. All rights reserved.
 *
 * see: http://www.ibm.com/developerworks/linux/library/l-openssl.html
 *      http://www.ibm.com/developerworks/linux/library/l-openssl2.html
 *      http://www.ibm.com/developerworks/linux/library/l-openssl3.html
 *
 */

#include "s_server.h"

#include<unistd.h>
#include<openssl/bio.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<assert.h>
#include<pthread.h>
#include<libgen.h>
#include<string.h>
#include<signal.h>  /* added 30 mar 2012 for ubuntu complile */

#define DEFAULT_PORT "8080"
/* #define HELLO_MESSAGE NULL */
#define HELLO_MESSAGE "HELLO\r\n" 
/* #define ECHO_PREFIX NULL  */
#define ECHO_PREFIX "you said: "

#define CERT_FILE "cert.pem"

int verbose = 0 ;


void print_usage(char * s) {
	/* warning, basename might modify actual argument s[...] */
	printf("usage: %s [-p servicename | -p port][-v]\n",basename(s)) ;
	return ;
}

void read_writer(BIO * bio) {
	if (verbose) printf("in read-writer\n") ;
	
	int i ;
	char buf[2048] ;
	if ( HELLO_MESSAGE ) BIO_write(bio, HELLO_MESSAGE, strlen(HELLO_MESSAGE)) ;
	while(1) {
		if ( (i=BIO_read(bio, buf, sizeof(buf)))<0 ) {
			printf("peer closed connection\n") ;
			ERR_print_errors_fp(stderr) ;
			exit(0) ;
		}
		buf[i] = '\0' ;
		if ( ECHO_PREFIX ) BIO_write(bio, ECHO_PREFIX, strlen(ECHO_PREFIX) ) ;
		BIO_write(bio, buf, strlen(buf)) ;
	}	
	return ;
}

int password_callback(char *buf, int size, int rwflag, void *userdata) {
    return 0 ;
}

static void sigpipe_handle(int x){
}

void apps_ssl_info_callback(const SSL *s, int where, int ret) { /* 30 mar 2012 added const */
        const char *str;
        int w;
printf("apps_ssl_info_callback: entered\n") ;
fflush(NULL) ;

        w=where & ~SSL_ST_MASK;

        if (w & SSL_ST_CONNECT) str="SSL_connect";
        else if (w & SSL_ST_ACCEPT) str="SSL_accept";
        else str="undefined";

        if (where & SSL_CB_LOOP)
                {
                //BIO_printf(bio_err,"%s:%s\n",str,SSL_state_string_long(s));
                }
        else if (where & SSL_CB_ALERT) {
                str=(where & SSL_CB_READ)?"read":"write";
                /* BIO_printf(bio_err,"SSL3 alert %s:%s:%s\n",
                        str,
                        SSL_alert_type_string_long(ret),
                        SSL_alert_desc_string_long(ret));
		*/
                } else if (where & SSL_CB_EXIT)
                {
                if (ret == 0)
                 /*       BIO_printf(bio_err,"%s:failed in %s\n",
                                str,SSL_state_string_long(s));
		*/
		;
                else if (ret < 0)
                        {
			/*
                        BIO_printf(bio_err,"%s:error in %s\n",
                                str,SSL_state_string_long(s));
			*/
                        }
                }
        }

int main(int argc, char * argv[]) {
	int ch ;
	char * pn ;
	char * po ;
	BIO * bio, * abio ;
	SSL_CTX * ctx ;
	SSL * ssl ;
	X509 * x509 ;
	
	SSL_library_init() ;
	SSL_load_error_strings() ;
	ERR_load_BIO_strings() ;
	OpenSSL_add_all_algorithms() ;
	
	/* Set up a SIGPIPE handler */
	signal(SIGPIPE,sigpipe_handle);

	pn = argv[0] ;
	
	po = DEFAULT_PORT ;
	while ((ch = getopt(argc, argv, "vp:h")) != -1) {
		switch(ch) {
			case 'p':
				po = optarg ;
				break ;
			case 'v':
				verbose = 1 ;
				break ;
			case 'h':
			case '?':
				print_usage(pn) ;
				exit(0) ;
		}
	}
	argc -= optind ;
	argv += optind ;
	
	if ( argc!=0 ) {
		print_usage(pn) ;
		exit(0) ;
	}
	
	if ( verbose) printf("port is %s\n", po) ;
			
    ctx = SSL_CTX_new(SSLv23_server_method()) ;
    SSL_CTX_set_info_callback(ctx, apps_ssl_info_callback) ;

    if ( ctx==NULL ) {
    	fprintf(stderr, "Failed SSL_CTX create\n") ;
		ERR_print_errors_fp(stderr) ;
		exit(0) ;
    }
    SSL_CTX_set_default_passwd_cb(ctx, password_callback);
    SSL_CTX_use_certificate_file(ctx,CERT_FILE,SSL_FILETYPE_PEM) ;
    SSL_CTX_use_PrivateKey_file(ctx,CERT_FILE, SSL_FILETYPE_PEM) ;
    
    bio = BIO_new_ssl(ctx, 0);
    if ( bio==NULL ) {
    	fprintf(stderr, "Failed bio create\n") ;
		ERR_print_errors_fp(stderr) ;
		exit(0) ;
    }
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    abio = BIO_new_accept(po);
    BIO_set_accept_bios(abio, bio);
	
	/* First call to BIO_accept() sets up accept BIO */
	if(BIO_do_accept(abio) <= 0) {
		fprintf(stderr, "Error setting up accept\n") ;
		ERR_print_errors_fp(stderr) ;
		exit(0) ;
	}
	
	while(1) {
		BIO * cbio ;
		if (BIO_do_accept(abio)<=0 ) {
			fprintf(stderr, "Error accepting connection\n") ;
			ERR_print_errors_fp(stderr) ;
			exit(0) ; 
		}
		cbio = BIO_pop(abio) ; /* pop off and catch the connect bio, reset underlying bio to an accept bio */

		if  (BIO_do_handshake(cbio)<=0 ) {
		    fprintf(stderr, "Error in handshake\n") ;
		    ERR_print_errors_fp(stderr) ;
		    exit(0) ;
		}
		
		if (fork()==0) {
			/* fork new process to handle rest of connection. I could have done a thread here, but new
			 processes is more traditional */
			read_writer(cbio) ; /* call a procedure, to keep this code short */
			if (verbose) printf("exiting child\n") ;
			exit(0) ;
		}
		/* wait on additional incoming connections */
	}
	assert(0==1) ;
}

