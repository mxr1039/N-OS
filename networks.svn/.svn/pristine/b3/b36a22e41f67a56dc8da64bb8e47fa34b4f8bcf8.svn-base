


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "arrange-h-e.h"

/*
 * author: burt, Francisco Delger
 * date: 27 jan 2016
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change name and continue work
 */


/* globals go here */
int g_debug = 0 ;


/* defines go here, in SHOUTY_CASE */
#define USAGE_MESSAGE "usage: arrange-h-e [-v] arg" 
#define PARSE_CHAR ":"


struct Node* create_node(void) {
  struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
	return newNode ;
}


void destroy_node(struct Node* n) {
        free(n); 
	return ;
}

void push (struct Node** head, char* token);
void printNodes (struct Node* head);
 

int main(int argc, char* argv[]) {

	int is_verbose = 0 ;
        char* delimiter = ":";        
	int ch ;        
      	assert(argv[1]);	 		    
	char* msg = strdup(argv[1]);
	char* token = (char*) malloc(sizeof(char));
        
	// starter code for using getopt
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
			case 'v':
				is_verbose = 1 ;
				break ;
			default:
				printf("%s\n", USAGE_MESSAGE) ;
			return 0 ;
		}
	}

	argc -= optind;
	argv += optind;
	
	if ( argc==0  ) {
		printf("%s\n", USAGE_MESSAGE) ;
		return 0 ;
	}
        
	// an example of an assertion
	assert(msg);
	// an example of if is_verbose
	if ( is_verbose ) {
	  printf("%s:%d: argument is |%s|\n", __FILE__, __LINE__, argv[2]);
	}

	// initialize list                                                      
        struct Node* head = NULL;              
        token = (char*) malloc(sizeof(char));
        token =  strtok(msg, delimiter);

        if(token == NULL)
	  exit (0);

        push(&head, token);
                 
 	while (token != NULL) {
          token = strtok (NULL, delimiter);
            if (token == NULL) {
	      printNodes(head);
              printf("\n");
	      exit(0);
	    }
          push(&head, token);
	} 
           
	return 0 ;// never gets here, stops at exit(0) once it gets to line 101
} 
void push(struct Node** head_ref, char* token) {

  struct Node* newNode =  create_node();

  newNode->msg = token;

  newNode->next = (*head_ref);

  (*head_ref) = newNode;
}

void printNodes(struct Node* head) {

  if (head == NULL) {
    return;
  }
 
  if (head->next == NULL) {
    printf("%s", head->msg);
    destroy_node(head);
    return;
  }

  printf("%s:", head->msg);
  destroy_node(head);
  printNodes(head->next);

}
