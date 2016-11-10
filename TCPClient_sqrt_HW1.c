/* TCP client to find square root of the given number */

#include <stdio.h>
#define _USE_BSD 1
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define BUFSIZE 64

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif





extern int errno;
int errexit(const char *format,...);
int connectTCP(const char *service,const char *host);
int connectsock(const char *service,const char *host,const char *transport);

/*------------------------------------------------------------------------------------
 * connectsock-Allocate and connect socket for TCP
 *------------------------------------------------------------------------------------
*/

int connectsock(const char *service,const char *host,const char *transport)
{

/*
Arguments:
*service   - service associated with desired port
*host      - name of the host to which connection is desired
*transport - name of the transport protocol to use

*/ 

struct sockaddr_in sin; 					 //an internet endpoint address
int s,type;               					 //socket descriptor and socket type 

memset(&sin,0,sizeof(sin));  
sin.sin_family=AF_INET;   				         //family name
  
sin.sin_port=htons(8000);                                        //port number


inet_pton(AF_INET,host,&(sin.sin_addr));                         //to convert host name into 32-bit IP address
 
/*
 * to determine the type of socket
 */

if(strcmp(transport,"udp")==0)         
type=SOCK_DGRAM;
else
type=SOCK_STREAM;

/* Allocate a socket */

s=socket(AF_INET,type,0);


if(s<0)
errexit("can't create socket : %s\n",strerror(errno));


if((connect(s,(struct sockaddr *) &sin,sizeof(sin)))<0)        //connect the socket 
errexit("can't connect to %s.%s: %s\n",host,service,strerror(errno));


return s;

}

/*
 * errexit- print and error message and exit
 */


int errexit(const char* format,...)
{
va_list args;

va_start(args,format);
vfprintf(stderr,format,args);
va_end(args);
exit(1);
}


/*------------------------------------------------------------------------
 * connectTCP-connect to a specified TCP service on specified host
 -------------------------------------------------------------------------*/

int connectTCP(const char *service,const char *host)
{
/*
 Arguments:
 *service-service associated with desired port
 *host-name of the host to which connection is desired
 */

return connectsock(service,host,"tcp");
}


/*
 main - TCP client for finding square root by modifying echo service
 */ 

int main(int argc,char *argv[])
{

char *host;
char sendline[100];
char recvline[1000];
char *number;
double result;


char *service="time";                                               //default service port

int s,n;                                                            //socket descriptor

switch(argc)
{
case 1:
host="localhost";

break;

case 2:
host="localhost";

break;

case 3:
number=argv[2];
host=argv[1];
break;

default:printf("Error in taking arguments\n");
exit(1);

}

result=atof(number);                                               //converting the number recieved as argument to double type

s=connectTCP(service,host);                                        //connect the socket


write(s,number,strlen(number)+1);                                  //write the number(send to server) for which the square root needs to be find

n=read(s,recvline,1000);                                           //read the message(square root of number from server) from server

if(n<0)
{
printf("read failed\n");
}


printf("Entered number is : %s\n",number);

if(result>0)
{
 printf("Square root of given number : %s\n",recvline);
}
else if(result<0)
{
  printf("%s\n",recvline);
}
else if(result==0)
{
printf("Square root of given number : %s\n",recvline);

}
else
{
 printf("%s\n",recvline);
}

exit(0);

}






















