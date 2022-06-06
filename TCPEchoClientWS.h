#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */
#include <Windows.h>
#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */


extern SYSTEMTIME st;

extern int sock;							/* Socket descriptor */
extern struct sockaddr_in echoServAddr;	/* Echo server address */
extern unsigned short echoServPort;		/* Echo server port */
extern char *servIP ;
extern char *echoString;
extern char echoBuffer[RCVBUFSIZE];		/* Buffer for echo string */
extern int echoStringLen;					/* Length of string to echo */
extern int bytesRcvd, totalBytesRcvd;		/* Bytes read in single recv() and total bytes read */
extern WSADATA wsaData;					/* Structure for WinSock setup communication */
    
extern void DieWithError(char *errorMessage);  /* Error handling function */
extern void creatConnection();
void sendMessenger(char* str);
extern void retreiveMessage();
extern void releaseConnection();