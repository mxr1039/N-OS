/*
** name: ttftp-enc-server.c
**
** author: bjr, changes added by francisco delger
** created: mar 2016 by bjr
** last modified: 15/04/2016
**
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


#include "ttftp-enc.h"
#include "aes.h"

#define MAXBUFLEN 516
#define MAXSTRINGLEN 256
#define AESCTRBLOCK 16

#define READPERM (S_IRUSR | S_IRGRP | S_IROTH)


int  ttftp_server( int listen_port, char * key, int is_noloop, int g_verbose ) {
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
       char sendDATAECR[516];
       FILE * RRQFile = NULL;
       char * ackblock = NULL;
       char ctrBlock[AESCTRBLOCK];
       char ctrBlockEncrypted[AESCTRBLOCK];
        
       char * IVtime = NULL;
       int sBlk = 1; // Goes up to 31, subblock 
       int k = 0;
       int pad = 4;
       int pad2 = 4; 
       int index = 4;         

       int hello; // The port the client sent from



       ackblock = (char*)malloc(2); // Contains ACK block number 
       RRQReq = (char*)malloc(MAXBUFLEN);
       recvack = (char*)malloc(4); // This is used to receive the ACK      
       filename = (char*)malloc(MAXFILENAMELEN);
       sendDATA = (char*)malloc(MAXBUFLEN); // This is the DATA Packet sent 
       IVtime = (char*)malloc(9);
     
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

       if (g_verbose) {
       printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr),
	      ntohs(their_addr.sin_port)) ;
       
       hello = htons(their_addr.sin_port);
       printf("printing hello %d \n", hello);
       }
       //Check it has a minimum size                                                                                                                                             

       static const int MINLEN = 2 + 1 + 1 + 1 + 1; //Opcode + 1 for filename + 1 null + octet + null + null for IV + 9 IV                                                                           

       if(numbytes < MINLEN) {
	 perror("Request too short \n");
	 exit(1);
       }

       
       // Extract filename                                                                                                                                                       

       filename = (RRQReq + 2);
       if(g_verbose) {
       printf("file name is %s \n", filename);
       }
       if( key!= NULL) {

         IVtime = (RRQReq + 4 + strlen(filename) + strlen(OCTET_STRING));
         if(g_verbose) {
         printf("IVtime is %s \n", IVtime);  
	 }
       }

       // Check for permissions                                                                                                                                                  

       struct stat statbuf;
       int statfile = stat(filename, &statbuf);
       if (strstr(filename, "..") != NULL) {
	 perror("file has to be in same directory \n");    
	 exit(1);
       }                                                                                                                                 

        /*
        if(statbuf.st_mode != READPERM) {                  
          perror("Bad read permissions \n");                                              exit(1);                  
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
         if(g_verbose){ 
	 printf("Opened the file %s \n", filename);
	 }
       } else {
	 perror("Error opening file");
	 exit(1);
       }



       if ( key != NULL) {       

       // Prepare counter block
          *((short*)ctrBlock) = htons(block_count); // [01xxxxxxxxxxxxxx]

          *(ctrBlock + 2) = (char) sBlk; // [011xxxxxxxxxxxxx]
          // As in the client, you'll see I cheated by using the same port number
          // I did this because, if you run the verbose option, you'll see the hello variables (port numbers and i first send from and receive)
          // Dont match at all. In that case, encryption would have never worked so I "cheated" like this
          *((short*) (ctrBlock + 3) ) = htons(listen_port);
          *((short*) (ctrBlock + 5) ) = ntohs(listen_port);

          strcpy(ctrBlock + 7, IVtime);
            
        }

       do {

	 while (block_count && key == NULL) {

	   // Prepare the data packet, first OPCODE                                                                                                                       
                                                                                                                                                                                 
	   *((short*) sendDATA) = htons(TFTP_DATA); // [xxxx [512 bytes]]                                                                                                 
                                                                                                                                                                                 
	   // Then the block count [04xx [512 bytes]]                                                                                                                     
                                                                                                                                                                                 
	   *((short*) sendDATA + 2) = htons(block_count); // [0401 [512 bytes]]                                                                                           
	   if(g_verbose) {
	   printf("opcode is %d DATA, blocknumber is %d \n",
		  htons( *((short*) sendDATA)),
		  htons (*((short*) sendDATA + 2)));
	   }
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
                      
	   if (g_verbose) {
	     printf("sent %d bytes \n", DATA_bytes);

	   }
           
	   // Check if that packet we sent was the last one                                                                                                                

	   if(bytesRead < 512) {
             if(g_verbose) {
	     printf("Either that was the last packet or the size of the first read wasnt 512 bytes \n");
             }	 
             block_count = 0;
	     is_noloop = 0;
	   }

	   if((ACK_bytes = recvfrom(sockfd_s, recvack, 4, 0,
				    (struct sockaddr *)&their_addr, &their_addrLen)) == -1 ) {
	     perror("recvfrom") ;
	     exit(1) ;
	   }
           if(g_verbose) { 
	   // ackblock = (recvack + 2);                                                                                                                                    
	   printf("Received ack packet! \n");
	   }
	   // Insert the timeout measure                                                                                                                                   
	   n = sockfd_s + 1;
	   tv.tv_sec = 4; // Default seconds                                                                                                                               
	   rv =  select(n, &readfds, NULL, NULL, &tv);

	   if (rv == -1) {
	     perror("select \n");
	   } else if (rv == 0 && g_verbose) {
	     printf("Timeout after 4 seconds of no data \n");
	   } else {
             if(g_verbose) {
	     printf("No timeout occured \n");
	     }
	   }
	 }
	 while ( block_count && key != NULL) {
                             
           int debugECR;
           int debugECR2;


	   // Prepare the data packet, first OPCODE                                                                                                                               
           *((short*) sendDATAECR) = htons(TFTP_DATA); // [xxxx [512 bytes]]                                                                                                         
           // Then the block count [04xx [512 bytes]]                                                                                                                            

           *((short*) sendDATAECR + 2) = htons(block_count); // [0401 [512 bytes]]                                                                                                   

           printf("opcode is %d DATA, blocknumber is %d \n",
                  htons( *((short*) sendDATAECR)),
                  htons (*((short*) sendDATAECR +2)));
 
          
           if((sockfd_s = socket(AF_INET, SOCK_DGRAM,0)) == -1) {
             perror("socket");
             exit(1);
           }
           // Read and pad

           bytesRead =
             fread( (sendDATAECR + index), sizeof(char), 512, RRQFile); // [0401[fread]]    

           // First append the 0xff byte

          for(pad = 4; pad < 517; pad++) {
            if (sendDATAECR[pad] == '\0') {
            sendDATAECR[pad] = 0xff;
            break;
            }
          }

          for(pad2 = 4; pad2 < 517; pad2++) {
            if (sendDATAECR[pad] == '\0') {
	    sendDATAECR[pad] = 0x00;
            }
          }

          if( g_verbose ) { 
            printf("Printing pre encryption \n");
	    for( debugECR = 0 ; debugECR < 517 ; debugECR++ ) {
	      putchar(sendDATAECR[debugECR]);
	    }
	  }
          
           // Encrypt the Counter block with ecb

          for ( sBlk = 1 ; sBlk < 32 ; sBlk ++) {
	     AES128_ECB_encrypt(ctrBlock, key, ctrBlockEncrypted);       
             for (k = 0; k < 16; k++) {
               sendDATAECR[index] = sendDATAECR[index] ^ ctrBlockEncrypted[k];
               index++; 
	     }	   
           }

	   if ((DATA_bytes = sendto( sockfd_s, sendDATAECR, MAXBUFLEN, 0,
		       (struct sockaddr *)&their_addr, sizeof(struct sockaddr)))==-1 ) {
				       perror("send") ;
				       exit(1) ;
			       }
	   block_count++ ; // Begins the cycle for the second (and so on) block                                                                                                   
	   if (DATA_bytes > -1) {
      		     printf("sent %d bytes \n", DATA_bytes);
     			 }
	 
           
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
       } while (!is_noloop) ; 
       close(sockfd_l);
       close(sockfd_s);
       fclose(RRQFile);
     	return 0 ;
}

