/*
** name: ttftp-client.c
**
** author: pikachu / Francisco Delger
** created: 31 jan 2015 by bjr
** last modified: 26/03/2016
**		14 feb 2016, for 162 semester of csc424 -bjr 
**
** from template created 31 jan 2015 by bjr
** Issues known: For some reason >> or > file-on-client.txt doesnt redirect
** the file stays empty. But the program does transmit, manual testing
** without file-on-client gives me good output
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

#include "ttftp.h"

#define h_addr h_addr_list[0]
#define MAXDATALEN 512
#define MAXBUFLEN 516
#define MAXFILENAME 255
#define FIRSTBLOCK 0
#define MAXTIMEOUTS 4 // number of attempts
#define TIMEOUT 4  // number of seconds

int ttftp_client( char * to_host, int to_port, char * file ) {

	int block_count ; 
        int sockfd;
        struct sockaddr_in my_addr;
        struct hostent *he;
        struct sockaddr_in their_addr;
        struct sockaddr_in recv_addr;
        static const char * mode = "octet";         	
        char * sendbuf  = NULL;
        char * sendack  = NULL;
        char  recvbuf [MAXBUFLEN];
        int msg_size = 0;
        int ack_size = 4;
       unsigned int addrLen;
        int numbytes = 0;
        int ackbytes = 0;
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

         msg_size = 2 + strlen(file) + strlen(OCTET_STRING) + 2;

	 sendbuf = (char*)malloc(msg_size);

         *((short*) sendbuf) = htons(TFTP_RRQ);

         strcpy(sendbuf + 2, file);
         strcpy(sendbuf + 2 + strlen(file) + 1, OCTET_STRING);

         printf("opcode is %d, filename is %s, OCTET_STRING is %s \n", 
		htons( *((short*) sendbuf)), sendbuf + 2, sendbuf + 3 + strlen(file)); 
       
      //Send RRQ
          if ((numbytes = sendto(sockfd, sendbuf, msg_size, 0,
	    (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1 ) {
	   perror("sendto");
           exit(1);
	  }
 
          printf("%d bytes were sent \n", numbytes);

	  block_count = 1; 


	while ( block_count ) {

          int i;
          addrLen = sizeof(struct sockaddr);

          if ((numbytes = recvfrom(sockfd, recvbuf, MAXBUFLEN, 0,	       		   (struct sockaddr *)&their_addr, &addrLen)) == -1) {
                    perror("recvfrom");
                    exit(1);	
          }
  
	    printf("Printing char... \n");
	               
             for(i = 4; i < 513; i++) {
              putchar(recvbuf[i]);
	    }
	    
	  printf("got packet from %s, port %d\n", inet_ntoa(their_addr.sin_addr),   ntohs(their_addr.sin_port)) ;

   	  printf("packet is %d bytes long\n", numbytes ) ;


	  // Bind with the ephemeral port
	  /*
	  if (their_addr.sin_port == htons(to_port)) {
	    their_addr.sin_port = recv_addr.sin_port;
	  }
	  */

          sendack = (char*)malloc(ack_size);  // [xxxx]

          *((short*) sendack) = htons(TFTP_ACK); // [04xx]

	  *((short*) sendack + 2) = htons(block_count);

   
	  printf("opcode is %d ACK, blocknumber is %d \n",
	      htons( *((short*) sendack)),  htons (*((short*) sendack + 2)));
	  

	  // Send the ack packet
         
	  if((ackbytes = sendto(sockfd, sendack, ack_size, 0,
	  (struct sockaddr *)&their_addr, sizeof(struct sockaddr)))  == -1) {
	       perror("sendto ack");
               exit(1);
	     }
	       block_count ++; // Finished one DATA (SERVER) > DATA (SERVER/CLIENT) > ACK (CLIENT/SERVER)

	       printf("%d ack bytes were sent \n", ackbytes);


          // Check if that was the last packet that needed to be sent

          if(numbytes < 516) {
             printf("That was the last packet that was needed, exiting... \n");
             block_count = 0;
          } 
	}
             close(sockfd);
		  return 0;
}

	    
