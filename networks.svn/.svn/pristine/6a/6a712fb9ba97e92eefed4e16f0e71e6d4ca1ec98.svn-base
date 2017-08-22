/*                                                                             
** name: twilio.c                                                              
**                                                                             
** author: francisco delger                                                 
** created:20/4/2016                                                     
** last modified: 24/04/2016                                                  
**                                                                  
** from cpp version by Laurent Luce                              
** WILL SUBMIT A FINAL VERSION LATER ON
** LIST AND GET WORKS WITH A STANDARD STRING
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<curl/curl.h>
#include "twilio.h"


#define URL "https://api.twilio.com/2010-04-01/Accounts/AC4e787f8a51b5ecaf00d23efff39a2517/Messages"
#define POSTURL "https://api.twilio.com/2010-04-01/Accounts/AC4e787f8a51b5ecaf00d23efff39a2517/Messages.json"

#define SAUTH "AC4e787f8a51b5ecaf00d23efff39a2517:e855b3ea165ecabd685776ad9df60ab3"

#define TWILIOID "AC4e787f8a51b5ecaf00d23efff39a2517"

#define TWILIO_NUMBER +17866554480 

#define TWILIOMESSAGE "HelloFran"

#define TWILIO_QUERY "https://api.twilio.com/2010-04-01/Accounts/AC4e787f8a51b5ecaf00d23efff39a2517/Messages/SMf64b1aad5df04aa6b77ef09b3993d7f7.json"


#define USAGE_MESSAGE "usage:  twilio [-v] -S twilio-sid  -T twilio-token [-R twilio-number] action [-m content of message to SEND arguments ... ]"

int g_verbose = 0 ;

int main(int argc, char * argv[]) {
  char * twilioID = NULL;
  char  * twilioTOKEN = NULL;
  long number = 0;
  char  * message = NULL;
  int ch;
  int is_delete = 0 ;
  int is_list = 0 ;
  char * buffer = (char*)malloc(256); 

      while ((ch = getopt(argc, argv, "vLDS:T:R:m:")) != -1) {
           switch(ch) {
                  case 'v':
                     g_verbose ++ ;
                     break ;
	          case 'D':
		    is_delete ++ ;
                    break;
	          case 'L':
                    is_list ++ ;
                    break;
                  case 'S':
                     twilioID = strdup(optarg) ;
                     break ;
                  case 'T':
                     twilioTOKEN = strdup(optarg) ;
                     break ;
                  case 'R':
                     number = atoi(optarg) ;
                     break ;
                  case 'm':
		    message = strdup(optarg) ;
                     break ;
      	          default:
		    printf("%s\n",USAGE_MESSAGE) ;
	             break ;
	   }
      }

      argc -= optind;
      argv += optind;

      if (g_verbose) { 
        printf("You have introduced the following arguments, ID is %s, Token is %s , message is %s , number is %d \n", twilioID, twilioTOKEN, message, number);
      }

      // Check if it's a list request
      
      if (is_list > 0 ) {
        printf("LIST request \n");
        buffer = twilio_LIST ( twilioID, twilioTOKEN );
        printf("%s \n", buffer);
      }

      if ( is_delete > 0) {
        printf("DELETE request \n");
        buffer = twilio_DELETE ( twilioID , twilioTOKEN ) ; // Working with static strings for now but ideally this
        printf(" %s \n", buffer); 
      }

      // Check if it's a send message request, so we check for request and number

      if ( number && message ) {
        printf("SEND request \n");
        buffer = twilio_SEND ( number , message );
        printf("%s \n", buffer);
      }

      // Check if it's a get

      if (message == NULL && !is_delete && !is_list) {
        printf("GET request \n");
        buffer = twilio_GET ( twilioID, twilioTOKEN );
        printf("%s \n", buffer);
      }


      return 0;



}


char * twilio_DELETE ( char * twilioID, char * twilioTOKEN ) {
  
  CURL * curl; 
  CURLcode res;
  char * deleteBuffer = "HTTP DELETE Request Success" ;
  char * failed = "DELETE Request failed";
  curl = curl_easy_init();
  FILE * fp;
  fp = fopen( "file.txt" , "w" );

  if(curl) {

    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, SAUTH);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl); 
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK ) {
      return deleteBuffer;
    }
    return failed;

  }

}

char * twilio_SEND ( int number , char * message ) {
  CURL *curl;
  CURLcode res;
   
  curl_global_init(CURL_GLOBAL_DEFAULT);
  char * sendBuffer = "HTTP POST SUCCESS";
  char * failed = "HTTP FAILED";
  curl = curl_easy_init();
  char * to = curl_easy_escape(curl, "To=+17863267330",15);
  char * from = curl_easy_escape(curl,  "From=+17866554480",17);
  char * body = curl_easy_escape(curl, "Body=hellofran",14);  
 
   if(curl) {

    curl_easy_setopt(curl, CURLOPT_URL, POSTURL);
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, SAUTH);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, to);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, from);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    res = curl_easy_perform(curl);

    if (res == CURLE_OK ) {
      return sendBuffer;
    }
    return failed;

  }
}

char * twilio_GET ( char * twilioID , char * twilioTOKEN) {

  CURL *curl;
  CURLcode res;
  char * failed = "HTTP GET FAILED";
  char * success = "HTTP GET SUCCESS";
  FILE * fp;
  fp = fopen( "file.txt" , "w" );

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, TWILIO_QUERY);
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, SAUTH);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK ) {
      return success;
    }
    return failed;





  }
}

char * twilio_LIST ( char * twilioID, char * twilioTOKEN ) {

  CURL *curl;
  CURLcode res;
  char * failed = "HTTP LIST FAILED";
  char * success = "HTTP LIST SUCCESS CHECK file.txt";
  FILE * fp;
  fp = fopen( "file.txt" , "w" );

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, POSTURL);
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, SAUTH);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res == CURLE_OK ) {
      return success;
    }
    return failed;
  }
}
