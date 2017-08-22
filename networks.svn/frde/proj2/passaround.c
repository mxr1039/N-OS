/*
** name: passaround.c
**
** author: Francisco Delger 
** date: 20/02/2016
** last modified: 24/02/2016
**
** from template created 31 jan 2015 by bjr
** 
   KNOWN ISSUES: Listener has to be running before sender in order to work.*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>

#include "passaround.h"

#define LOCALHOST "localhost"
#define MAXMSGLEN 2048
#define N_REPEAT_DEFAULT 1
#define MAXBUFLEN 100
#define USAGE_MESSAGE "usage: passaround [-v] [-n num] [-m message] port"

int g_verbose = 0 ;

int main(int argc, char * argv[]) {
	int ch ;
	int the_port = 0 ;
	int loop = 1 ;
	int n_repeat = N_REPEAT_DEFAULT ;
        char * host = LOCALHOST;
        char * msg = NULL;    
        char * s = NULL;     
        char * s2 = NULL;
        char * temp = NULL;

	int is_forever = 0 ;
        int sockfd;
        int sockdf;
        struct sockaddr_in my_addr;
        struct sockaddr_in their_addr;
	struct hostent *he;
        unsigned int addr_len, numbytes;
        
        char * buf;
        char * buf2;  

	assert(sizeof(short)==2) ; 
     
	while ((ch = getopt(argc, argv, "vm:n:")) != -1) {
		switch(ch) {
		case 'n':
			n_repeat = atoi(optarg) ;
			break ;
		case 'v':
			g_verbose = 1 ;
			break ;
		case 'm':
			msg = strdup(optarg) ;
			break;
		default:
			printf(USAGE_MESSAGE) ;
			return 0 ;
		}
	}

     
    argc -= optind;
    argv += optind;
    
    if ( argc!= 1 ) {
    	fprintf(stderr,"%s\n",USAGE_MESSAGE) ;
    	exit(0) ;
    }
    
    the_port = atoi(*argv) ;
    assert(the_port);

    if ((sockfd=socket(AF_INET, SOCK_DGRAM,0))== -1) {
      perror("socket");
      exit(1);
    }

    //    sockdf=socket(AF_INET, SOCK_DGRAM,0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons((short)the_port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);
  
   if (bind(sockfd, (struct sockaddr *)&my_addr,
	     sizeof(struct sockaddr)) == -1 ) {
                 perror("bind");
                 exit(1);
		 }
   
    while (msg && n_repeat > 0 ) {
          
        
	  buf2 = (char*) malloc(MAXBUFLEN+1) ;       

          s2 = strtok(msg, ":");

          if (s2 == NULL) {
            close(sockfd);
	    exit(0); // Nothing else to do, we're done
	  }         

          if ((he=gethostbyname(s2))==NULL) {	   
                perror("gethostbyname") ;
	       exit(1) ;
           }

          s2 = strtok(NULL, "\0");
          if (s2 == NULL) {
	    s2 = "";
	  }

     	  their_addr.sin_family = AF_INET ;
	  their_addr.sin_port = htons((short)the_port);
          their_addr.sin_addr = *((struct in_addr *)he->h_addr) ;
	  memset(&(their_addr.sin_zero), '\0',8);


          if ((numbytes=sendto(sockfd, s2, strlen(s2),0,
			       (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) ) == -1 ) {
	    perror("sendto") ;
	    exit(1) ;
	  }
	  
	  addr_len = sizeof(struct sockaddr_in) ;
	  getsockname( sockfd, (struct sockaddr *)&my_addr, &addr_len ) ;
	  if ( g_verbose ) {
	    printf("sent from port %d\n", ntohs(my_addr.sin_port)) ;
	    printf("calling for return packet\n") ;
	  }

	  if (!g_verbose) {
	    printf("S: %s:%d |%s| \n", inet_ntoa(their_addr.sin_addr),
		   ntohs(their_addr.sin_port), s2);
	  }
          if (s2 == "") {                      
            break;
	  }
          
	  addr_len = sizeof(struct sockaddr_in) ;
	  if ((numbytes=recvfrom(sockfd, buf2, MAXBUFLEN,0,
	       (struct sockaddr *)&their_addr, &addr_len)) == -1 ) {
	    perror("recvfrom") ;
	    exit(1) ;
	  }
	  
	  //  if (buf2 == NULL) {
          //  break;}
 
 
	  if ( g_verbose ) {
	    printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr), 
		   ntohs(their_addr.sin_port)) ;
	    printf("packet is %d bytes long\n", numbytes ) ;
	  }

	  if (!g_verbose) {
	    printf("R: %s:%d |%s| \n", inet_ntoa(their_addr.sin_addr),
		   ntohs(their_addr.sin_port), buf2);
	  }
      	  msg = buf2;
          n_repeat--;
       } 	
is_forever = (n_repeat == 0) ;

while( is_forever || n_repeat ) { 	
      // listen for a packet
      int bytes_sent;
      addr_len = sizeof(struct sockaddr);
      buf = (char*) malloc(MAXBUFLEN) ;
      if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN-1,0,
	 (struct sockaddr *)&their_addr, &addr_len)) == -1 ) {
	perror("recvfrom") ;
	exit(1) ;
      }
        // msg = buf;
      if ( g_verbose ) {
	printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr), 
	       ntohs(their_addr.sin_port)) ;
	printf("packet is %d bytes long\n", numbytes ) ;
	
        }
       if (!g_verbose) {
          printf("R: %s:%d |%s| \n", inet_ntoa(their_addr.sin_addr),
                 ntohs(their_addr.sin_port), buf);
        }
       if (buf == "") {
	 break;
       }
  
      {
     	int bytes_sent ;
        s = strtok(buf, ":"); //  cut first name to know where to send back
        if (s == NULL) {
          break;
	}

        he = gethostbyname(s); // update who we're sending to
        s = strtok(NULL, "\0");// update what we're sending them to the rest

        if (s == NULL) { // We have reached last packet, so we send empty
	  s = "";
	} 

	if ((bytes_sent = sendto( sockfd, s, strlen(s), 0,
	  (struct sockaddr *)&their_addr, sizeof(struct sockaddr)))==-1 ) {
	  perror("send") ;
	  exit(1) ;
	}
       
        if ( g_verbose ) {
	  printf("packet sent, %d bytes\n", bytes_sent) ;
	}
         
        if (!g_verbose) {
          printf("S: %s:%d |%s| \n", inet_ntoa(their_addr.sin_addr),
		 ntohs(their_addr.sin_port), s);
	}
        if (s == "") {
          close(sockfd);
          exit(0); // We have sent the last packet, done.
	}
      }
 }
 return 0 ; // never gets here, reaches exit through s=="" if
}

