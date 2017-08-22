/*
 *  s_client.c
 *
 *  sample ssl client using SSL, BIO and X509 API's
 *
 *  Created by Burton  Rosenberg on 3/31/11.
 *  Copyright 2011  Burton  Rosenberg. All rights reserved.
 *
 */

#include<unistd.h>
#include<openssl/bio.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<assert.h>
#include<pthread.h>
#include<libgen.h>

#define DEFAULT_SERVICE "http"

int verbose = 0 ;

void print_usage(char * s) {
	/* warning, basename might modify actual argument s[...] */
	printf("usage: %s [-p servicename | -p port ] hostname\n",basename(s)) ;
	return ;
}

void * socket_reader(void * arg) {
	/* not sure of the locking implications. I'll be sleeping to 
	   complete a read in one thread, and writing to the same BIO in
	   another thread. 
	*/
	BIO * bio = (BIO *) arg ;
	int i ;
	char buf[2048] ;
	while(1) {
		if ( (i=BIO_read(bio, buf, sizeof(buf)))<=0 ) {
			printf("peer closed connection\n") ;
			exit(0) ;
		}
		buf[i] = '\0' ;
		printf("%s",buf) ;
	}	
}

int examine_cert(SSL * ssl) {
    X509 * x509 ;
    X509_NAME * name ;
    int result ;
    STACK_OF(X509) * sox509 ;
    int i, n  ;
    
    result = SSL_get_verify_result(ssl) ; // see man verify
	printf ("verify: %d\n", result) ;
	
	// x509 = SSL_get_peer_certificate(ssl) ;
	// 509_NAME_get_text_by_NID(name, NID_commonName, commonName, 512);
	// X509_free(x509) ;
	
	sox509 = SSL_get_peer_cert_chain(ssl) ;
	n = sk_X509_num(sox509) ;
	for ( i=0; i<n; i++ ) {
	    printf("\nCert number %d\n",i+1) ;
	    x509 = sk_X509_value(sox509,i) ;
	    name = X509_get_subject_name(x509) ;
	    printf("  Name: %s\n",X509_NAME_oneline(name,NULL,0)) ;
	    name = X509_get_issuer_name(x509) ;
        printf("  Issuer: %s\n",X509_NAME_oneline(name,NULL,0)) ;
	}
    printf("\n") ;
    
    
    return result ;
}

int main(int argc, char *argv[]) {
	BIO * bio ;
	BIO * bio_t ;
	SSL_CTX * ctx ;
	SSL * ssl ;

	char buf[2048] ;
	char buf_r[2048] ;
	char * hn, * po ;
	pthread_t pth ;
	int i ;
	int ch ;
	char * pn ;
	
	SSL_library_init() ;
	SSL_load_error_strings() ;
	ERR_load_BIO_strings() ;
	OpenSSL_add_all_algorithms() ;
	
	pn = argv[0] ;
	
	po = DEFAULT_SERVICE ;
	hn = NULL ;
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
	
	if (argc==0) {
		print_usage(pn) ;
		exit(0) ;
	}
	hn = argv[0] ;
	
	ctx = SSL_CTX_new(SSLv23_client_method()) ;
	
	bio = BIO_new_ssl_connect(ctx) ;
	BIO_get_ssl(bio, &ssl) ;
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY) ;
	
	BIO_set_conn_hostname( bio, hn ) ;
	BIO_set_conn_port( bio, po ) ;
	if ( BIO_do_connect(bio)<=0 ) {
		printf("error opening connection to %s:%s\n", hn, po ) ;
		exit(1)  ;
	}
	
	examine_cert(ssl) ;
	
	if ( pthread_create(&pth, NULL, socket_reader, (void *) bio )!=0 ) {
		perror("pthread_create:") ;
		exit(0) ;
	}

	bio_t = BIO_new_fd(fileno(stdin), BIO_NOCLOSE);
	while(1) {
		i = BIO_read(bio_t, buf, sizeof(buf)) ;
		if ( i<0 ) return ;
		buf[i] = '\0' ;
		BIO_puts(bio, buf) ;
	}
	assert(0==1) ;
}
