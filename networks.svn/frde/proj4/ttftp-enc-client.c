/*
** name: ttftp-enc-client.c
**
** author: pikachu
** created: mar 2016 by bjr
** last modified:
**		14 feb 2016, for 162 semester of csc424 -bjr 
**
** from template created 31 jan 2015 by bjr
**
*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>
#include<unistd.h>

#include "ttftp-enc.h"
#include "aes.h"
#define MAXDATALEN 512
#define MAXBUFLEN 516
#define MAXFILENAME 255
#define FIRSTBLOCK 0
#define MAXTIMEOUTS 4 // number of attempts                                     
#define TIMEOUT 4  // number of seconds      
#define AESCTRBLOCK 16

#define h_addr h_addr_list[0]

int ttftp_client( char * to_host, int to_port, char * key, char * file, int g_verbose ) {

       int block_count ;
       int sBlk;
       int IV;
       int sockfd;
       struct sockaddr_in my_addr;
       struct hostent *he;
       struct sockaddr_in their_addr;
       struct sockaddr_in recv_addr;
       static const char * mode = "octet";
       char * sendbuf  = NULL;
       char * sendack  = NULL;
       char  recvbuf [MAXBUFLEN];
       char  ctrBlock [AESCTRBLOCK];
       char  ctrBlockEncrypted [AESCTRBLOCK];

       int msg_size = 0;
       int ack_size = 4;
       unsigned int addrLen;
       int numbytes = 0;
       int ackbytes = 0;
       int mytime; 
       int end = 1;
       int hello;
       char * IVtime = (char*)malloc(9);
       FILE *ioFile;

       
       /*                                                                      
	* create a socket to send                                              
	*/

       if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	 perror("socket");
	 exit(1);
       }

       if((he=gethostbyname(to_host)) == NULL ) {
	 perror("gethostbyname");
	 exit(1);
       }

       their_addr.sin_family = AF_INET;
       their_addr.sin_port = htons(to_port);
       their_addr.sin_addr = *((struct in_addr *) he->h_addr);
       memset(&(their_addr.sin_zero), '\0', 8);
       
       /* Preparing RRQ, with sendbuf and strncpy                         
	* 
        */
       mytime = time(NULL) % 1000000000;
       if(g_verbose) {
       sprintf(IVtime, "%d\n", mytime);
       printf("IVtime is %s\n", IVtime);      
       }

       msg_size = 2 + strlen(file) + strlen(OCTET_STRING) + 2 + 1 + strlen(IVtime);

       sendbuf = (char*)malloc(msg_size);

       *((short*) sendbuf) = htons(TFTP_RRQ);

       strcpy(sendbuf + 2, file);
       strcpy(sendbuf + 2 + strlen(file) + 1, OCTET_STRING);
       strcpy(sendbuf + 2 + strlen(file) + 1 + strlen(OCTET_STRING) + 1, IVtime);

       if(g_verbose) {
       printf("opcode is %d, filename is %s, OCTET_STRING is %s, IVtime is %s\n",
	      htons( *((short*) sendbuf)), sendbuf + 2, sendbuf + 3 + strlen(file), 
                         sendbuf + 4 + strlen(file) + strlen(OCTET_STRING) );
       }

       //Send RRQ                                                                                             
       if ((numbytes = sendto(sockfd, sendbuf, msg_size, 0,
		       (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1 ) {
	 perror("sendto");
	 exit(1);
       }

       

       if(g_verbose) {
	 printf("sent RRQ packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr),
		htons(my_addr.sin_port)) ;
         hello = htons(my_addr.sin_port);
	 printf("%d bytes were sent and hello is %d \n", numbytes, hello);
       }
       block_count = 1;

       if ( key != NULL) {

	 // Prepare counter block                                                                                                                                                   
	 *((short*)ctrBlock) = htons(block_count); // [01xxxxxxxxxxxxxx]                                                                                                         

	 *(ctrBlock + 2) = (char) sBlk; // [011xxxxxxxxxxxxx]                                                                                                                    

	 // *((short*) (ctrBlock + 3) ) = htons(hello); Can't get port number from client that I send from to match where server receives first,
         // Im using the same port number just to test the encryption.
         *((short*) (ctrBlock + 3) ) = htons(to_port);
	 *((short*) (ctrBlock + 5) ) = htons(to_port);

	 strcpy(ctrBlock + 7, IVtime);

       }

       while ( block_count > 0 && key == NULL) {

	 int i;
	 addrLen = sizeof(struct sockaddr);

	    if ((numbytes = recvfrom(sockfd, recvbuf, MAXBUFLEN, 0,                          
                       (struct sockaddr *)&their_addr, &addrLen)) == -1) {
	      perror("recvfrom");
	      exit(1);
	    }
 
            if (g_verbose) {
	     printf("Printing char... \n");
	    }

	    for(i = 4; i < 517; i++) {
	      if (recvbuf[4] == '\0') {
	       printf("File is completely empty");
	       break;
	      }

	      if ( recvbuf[i] == '\0' ) {
               end = 0;
	       break;
	      } 

	    putchar(recvbuf[i]);
	    }

	    if(g_verbose) {
	     printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr),   ntohs(their_addr.sin_port)) ;

	     printf("packet is %d bytes long\n", numbytes ) ;
	   }

	 // Bind with the ephemeral port                                           
	   //   if (their_addr.sin_port == htons(to_port)) {                  
	   // their_addr.sin_port = recv_addr.sin_port;                    
	   //	 }                                                                   
        

	   sendack = (char*)malloc(ack_size);  // [xxxx]                                                                                                                           

	   *((short*) sendack) = htons(TFTP_ACK); // [04xx]                                                                                                                        

	   *((short*) sendack + 2) = htons(block_count);

	   if(g_verbose) {
	   printf("opcode is %d ACK, blocknumber is %d \n",
		  htons( *((short*) sendack)),  htons (*((short*) sendack + 2)));
	   }
	   // Send the ack packet                                                                                                                                                  

	   if((ackbytes = sendto(sockfd, sendack, ack_size, 0,
				 (struct sockaddr *)&their_addr, sizeof(struct sockaddr)))  == -1) {
	     perror("sendto ack");
	     exit(1);
	   }
	   block_count ++; // Finished one DATA (SERVER) > DATA (SERVER/CLIENT) > ACK (CLIENT/SERVER)                                                                         

           if(g_verbose) {
	   printf("%d ack bytes were sent \n", ackbytes);
	   }


	   // Check if that was the last packet that needed to be sent     
	   if(end == 0) {
             if(g_verbose) {
             printf("That was the last packet that was needed, didn't reach 516 bytes length of data, exiting... \n");
             }
             block_count = 0;
	   }
       }

       while ( block_count > 0 && key != NULL) {

	 int j;
         int k;
         int debug;
         int index = 4;
         addrLen = sizeof(struct sockaddr);

         if ((numbytes = recvfrom(sockfd, recvbuf, MAXBUFLEN, 0,
				  (struct sockaddr *)&their_addr, &addrLen)) == -1) {
           perror("recvfrom");
           exit(1);
         }

         if (g_verbose) {
	   printf("Printing char... \n");
           for( debug = 0; debug < 517; debug++)
	     putchar(recvbuf[debug]);
         } 
   
	 for ( sBlk = 1 ; sBlk < 32 ; sBlk ++) {
	   AES128_ECB_encrypt(ctrBlock, key, ctrBlockEncrypted);
	   for (k = 0; k < 16; k++) {
	     recvbuf[index] = recvbuf[index] ^ ctrBlockEncrypted[k];
	     index++;
	   }
	 }
       
	 printf("Printing postdecryption \n");
         for (j = 0 ; j < 517 ; j++) {
	   putchar(recvbuf[j]);
            if ( recvbuf[j] == '\0' ) {
              end = 0;  
	    }
	   
	 }
       

         if(g_verbose) {
	   printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr),   ntohs(their_addr.sin_port)) ;

	   printf("packet is %d bytes long\n", numbytes ) ;
         }

	 sendack = (char*)malloc(ack_size);  // [xxxx]                                                                                                                         \
                                                                                                                                                                                 
	 *((short*) sendack) = htons(TFTP_ACK); // [04xx]                                                                                                                      \
                                                                                                                                                                                 
	 *((short*) sendack + 2) = htons(block_count);

	 if(g_verbose) {
           printf("opcode is %d ACK, blocknumber is %d \n",
                  htons( *((short*) sendack)),  htons (*((short*) sendack + 2)));
	 }

	 if((ackbytes = sendto(sockfd, sendack, ack_size, 0,
			       (struct sockaddr *)&their_addr, sizeof(struct sockaddr)))  == -1) {
	   perror("sendto ack");
	   exit(1);
	 }
	 block_count ++; // Finished one DATA (SERVER) > DATA (SERVER/CLIENT) > ACK (CLIENT/SERVER)                                                                             

	 if(g_verbose) {
           printf("%d ack bytes were sent \n", ackbytes);
	 }


	 // Check if that was the last packet that needed to be sent                                                                                                            
	 if(end == 0) {
	   if(g_verbose) {
             printf("That was the last packet that was needed, didn't reach 516 bytes length of data, exiting... \n");
	   }
	   block_count = 0;
	 }
       }

       close(sockfd);
       return 0;
}

 







