#include "TCPEchoClientWS.h"

extern int	isRecalled;
SYSTEMTIME st;
int sock;							/* Socket descriptor */
struct sockaddr_in echoServAddr;	/* Echo server address */
//unsigned short echoServPort = 3490;	/* Echo server port */
unsigned short echoServPort = 8080;	/* Echo server port */
//char *servIP = "129.79.192.31";     /* Server IP address (dotted quad) */
//char *servIP = "129.79.192.22";     /* Server IP address (dotted quad) */
char *servIP = "127.0.0.1";
char echoBuffer[RCVBUFSIZE];		/* Buffer for echo string */
int echoStringLen;					/* Length of string to echo */
int bytesRcvd, totalBytesRcvd;		/* Bytes read in single recv() and total bytes read */
int bytesSend;                      /* Bytes send in single send() */
WSADATA wsaData;					/* Structure for WinSock setup communication */



void creatConnection()
{
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");
	sendMessenger("Testing\0");
}

void sendMessenger(char* str)
{
	//clock_t goal = 150 + clock();
	//while (goal > clock());
	if (send(sock, str, strlen(str), 0) != strlen(str))
	{
        DieWithError("send() sent a different number of bytes than expected");
	}
	printf("Sent: %s\n", str);	
}

void retreiveMessage()
{
	totalBytesRcvd = 0;
    printf("Received: ");                /* Setup to print the echoed string */
    while (totalBytesRcvd < echoStringLen)
    {
        /* Receive up to the buffer size (minus 1 to leave space for 
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        echoBuffer[bytesRcvd] = '\0';  /* Add \0 so printf knows where to stop */
        printf(echoBuffer);            /* Print the echo buffer */
    }

    printf("\n");    /* Print a final linefeed */
	GetSystemTime(&st);
	printf("retreiving back time:%d\n", st.wMilliseconds);
}

void releaseConnection()
{
	isRecalled = 1;
	closesocket(sock);
    WSACleanup();  /* Cleanup Winsock */
}