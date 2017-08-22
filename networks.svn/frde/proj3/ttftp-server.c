/*
** name: ttftp-server.c
**
** author: bjr %% francisco delger
** created: 14 feb 2016 by bjr
** last modified:
** Have yet to fix many things.
** from extracted from ttftp.c
*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>
#include<unistd.h>
#include <sys/stat.h>

#include "ttftp.h"

#define MAXBUFLEN 516
#define MAXSTRINGLEN 256

#define READPERM (S_IRUSR | S_IRGRP | S_IROTH)


int  ttftp_server( int listen_port, int timeout, int is_noloop ) {
  	int sockfd_l;
	int sockfd_s ;
        int n; // Contain socket_s + 1 for select timeout
        int rv; // Select function
        fd_set readfds;
        size_t bytesRead = 0;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
        struct timeval tv; // Timeout struct
        unsigned int addrLen, numbytes;
   	int block_count = 0;
        int DATA_bytes;
        int ACK_bytes;
        char * RRQReq = NULL; 
        char * recvack = NULL;
        char * filename = NULL;
        char * sendDATA = NULL;        
        FILE * RRQFile = NULL;
        char * ackblock = NULL;
    
        ackblock = (char*)malloc(2); // Contains ACK block number
        RRQReq = (char*)malloc(MAXBUFLEN);
        recvack = (char*)malloc(4); // This is used to receive the ACK
        filename = (char*)malloc(MAXFILENAMELEN);
        sendDATA = (char*)malloc(MAXBUFLEN); // This is the DATA Packet sent
     
	/*
	 * create a socket to listen for RRQ
	 */

        if((sockfd_l = socket(AF_INET, SOCK_DGRAM,0)) == -1) {
	  perror("socket");
          exit(1);
	}
      
        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        my_addr.sin_port = htons(listen_port);
        memset(&(my_addr.sin_zero),'\0' ,8);
        
        if(bind(sockfd_l, (struct sockaddr*)&my_addr,
		sizeof(struct sockaddr)) == -1 ) {
	        perror("bind");
                exit(1);
	}     
	
        // Listen for RRQ First
        socklen_t their_addrLen = sizeof(struct sockaddr);

        if((numbytes = recvfrom(sockfd_l, RRQReq, MAXBUFLEN-1, 0, 
	       (struct sockaddr *)&their_addr, &their_addrLen)) == -1 ) {
	  perror("recvfrom") ;
	  exit(1) ;
	}
       
        block_count++; // We received our first packet, the reading request

        printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr),
	       ntohs(their_addr.sin_port)) ;
      
	//Check it has a minimum size

	static const int MINLEN = 2 + 1 + 1 + 1 + 1; //Opcode + 1 for filename + 1 null + octet + null

	if(numbytes < MINLEN) {
	  perror("Request too short \n");
	  exit(1);
	}

	// Extract filename

        filename = (RRQReq + 2);

        printf("file name is %s \n", filename);

        // Check for permissions

	struct stat statbuf;                                                                                                                                                    
	int statfile = stat(filename, &statbuf);                                                                                                                               
	if (strstr(filename, "..") != NULL) {                                                                                                         
	  perror("file has to be in same directory \n");                                                                                                                           
	  exit(1);                                                                                                                                                              
	}                                                                                                                                                     /*                             
	if(statbuf.st_mode != READPERM) {                                                                                                                                       
	  perror("Bad read permissions \n");
          exit(1);                                                                                                                                  
	}           
																			      */																			       
	const size_t filenameLen = strlen(filename);                                                                                                                             
	const size_t octetLen = strlen(OCTET_STRING);                                                                                                                                                                                                                                                                                         
	if (strncasecmp((RRQReq + 2 + strlen(filename) + 1),OCTET_STRING,octetLen)) {                                                                                            
	  perror("only octet supported \n");
          exit(1);                                                                                                                                       
	} 
  
	// Open the file                                                      

	if ((RRQFile = fopen(filename, "r")) != NULL) {
	  printf("Opened the file %s \n", filename);
	} else {
	  perror("Error opening file");
	  exit(1);
	}

	
	do { 
    		while (block_count) { 

		  // Prepare the data packet, first OPCODE                                                                                                                                
		  *((short*) sendDATA) = htons(TFTP_DATA); // [xxxx [512 bytes]]                                                                                                          
		  // Then the block count [04xx [512 bytes]]                                                                                                                              
		  *((short*) sendDATA + 2) = htons(block_count); // [0401 [512 bytes]]                                                                                                    
		  printf("opcode is %d DATA, blocknumber is %d \n",
		    htons( *((short*) sendDATA)),  
                                        htons (*((short*) sendDATA + 2)));



		  // Read 512 bytes of the file                                                                                                                                           
		  bytesRead = 
                         fread( (sendDATA +4), sizeof(char), 512, RRQFile); // [0401[fread]]                                                                                                 
		  if((sockfd_s = socket(AF_INET, SOCK_DGRAM,0)) == -1) {
		    perror("socket");
		    exit(1);
		  }

		  if ((DATA_bytes = sendto( sockfd_s, sendDATA, MAXBUFLEN, 0,
			     (struct sockaddr *)&their_addr, sizeof(struct sockaddr)))==-1 ) {
		      perror("send") ;
		      exit(1) ;
		    }
		  block_count++ ; // Begins the cycle for the second (and so on) block               
 
                  if (DATA_bytes > -1) {
		    printf("sent %d bytes \n", DATA_bytes);

		  }

                  // Check if that packet we sent was the last one
 
                  if(bytesRead < 512) { 
                    printf("Either that was the last packet or the size of the first read wasnt 512 bytes \n");
                    block_count = 0;
                    is_noloop = 0;
		  }

		  if((ACK_bytes = recvfrom(sockfd_s, recvack, 4, 0,
					  (struct sockaddr *)&their_addr, &their_addrLen)) == -1 ) {
		    perror("recvfrom") ;
		    exit(1) ;
		  }

                  // ackblock = (recvack + 2);
      	          printf("Received ack packet! \n");

		  // Insert the timeout measure		    
                  n = sockfd_s + 1;
                  tv.tv_sec = 4; // Default seconds
                  rv =  select(n, &readfds, NULL, NULL, &tv);
                  if (rv == -1) {
                    perror("select \n");
		  } else if (rv == 0) {
                    printf("Timeout after 4 seconds of no data \n");
		  } else {
                    printf("No timeout occured \n"); 
		  }
		}
	} while (is_noloop) ;

        close(sockfd_l);
        close(sockfd_s);
        fclose(RRQFile);
	/*
        free(RRQReq);
        free(recvack);
        free(filename);
        free(sendDATA);
        free(RRQFile);
	*/
	return 0 ;
}
