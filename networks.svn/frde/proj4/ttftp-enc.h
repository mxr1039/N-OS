/*
** name: ttftp-enc.h
**
** author: bjr
** created: mar 2016 by bjr
** last modified:
**
** from template created 31 jan 2015 by bjr
*/

extern int g_verbose ;

#define MAXMSGLEN 2048
#define MAXFILENAMELEN 256

#define TFTP_RRQ 1
#define TFTP_WRQ 2
#define TFTP_DATA 3
#define TFTP_ACK 4
#define TFTP_ERR 5

#define OCTET_STRING "octet"
#define TFTP_DATALEN 512

#define ACK_TIMEOUT 4
#define ACK_RETRY 6

#define VAR_SIZE 0 

#define AES_BLK 16
#define IV_LEN 9
#define KEY_LEN 16

struct TftpReq {
	char opcode[2] ;
	char filename_and_mode[VAR_SIZE] ;
} ;

struct TftpReqEnc {
	char opcode[2] ;
	char filename_mode_and_IV[VAR_SIZE] ;
} ;

/* struct TftpReqEnc {
	char opcode[2] ;
	char filename_mode_and_IV[VAR_SIZE] ;
	} ; */

struct TftpData {
	char opcode[2] ;
	char block_num[2] ;
	char data[VAR_SIZE] ; /* zero to 512 bytes */
} ;

struct TftpAck {
	char opcode[2] ;
	char block_num[2] ;
} ;

struct TftpError {
	char opcode[2] ;
	char error_code[2] ;
	char error_msg[VAR_SIZE] ;
} ;


int  ttftp_client( char * host, int port, char * key, char * file, int g_verbose ) ;
int  ttftp_server( int listen_port, char * key, int is_noloop, int g_verbose ) ;

