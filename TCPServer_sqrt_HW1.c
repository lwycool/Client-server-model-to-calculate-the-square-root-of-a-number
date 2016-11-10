/* TCP server to find the square root of the given number */

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
#include <math.h>




int connectTCP(const char *service);
int connectsock(const char *service,const char *transport);


/*------------------------------------------------------------------------------------
 * connectsock-Allocate and connect socket for TCP
 *------------------------------------------------------------------------------------
*/

int connectsock(const char *service,const char *transport)
{
/*
Arguments:
*service   - service associated with desired port
*transport - name of the transport protocol to use
*/
struct sockaddr_in server;                                                //an internet endpoint address

int server_socket,type,b,l,accept_socket,num;                             //two socket descriptors for listening and accepting 

memset(&server,0,sizeof(server));

server.sin_addr.s_addr=htons(INADDR_ANY);                                 //INADDR_ANY to match any IP address
server.sin_family=AF_INET;                                                //family name
server.sin_port=htons(8000);                                              //port number

 
/*
 * to determine the type of socket
 */

if(strcmp(transport,"udp")==0)
{
type=SOCK_DGRAM;
}
else
{
type=SOCK_STREAM;
}


server_socket=socket(AF_INET,type,0);                                    //allocate a socket

if(server_socket<0)
{
printf("Socket can't be created\n");
exit(0);
}

/* to set the socket options- to reuse the given port multiple times */
num=1;

if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEPORT,(const char*)&num,sizeof(num))<0)
{
printf("setsockopt(SO_REUSEPORT) failed\n");
exit(0);
}


/* bind the socket to known port */
b=bind(server_socket,(struct sockaddr*)&server,sizeof(server));

if(b<0)
{
printf("Error in binding\n");
exit(0);
}


/* place the socket in passive mode and make the server ready to accept the requests and also 
   specify the max no. of connections
 */
l=listen(server_socket,10);
if(l<0)
{
printf("Error in listening\n");
exit(0);
}

return server_socket;

}



/*------------------------------------------------------------------------
 * connectTCP-connect to a specified TCP service on specified host
 -------------------------------------------------------------------------*/
int connectTCP(const char *service)
{
/*
 Arguments:
 *service-service associated with desired port
 */
 return connectsock(service,"tcp");
}


/*
 main - TCP server for finding square root by modifying echo service
 */

int main(char argc,char *argv[])
{

char msg[1000];
char *service="echo";
char errmsg[1000]="Entered number is negative.Please enter positive number";
char errmsg1[1000]="Enter valid number";
double number;
double result;

int accept_socket,sock;
struct sockaddr_in fsin;

/* call connectTCP to create a socket, bind it and place it in passive mode
   once the call returns call accept on listening socket to accept the incoming requests
 */

sock=connectTCP(service);
accept_socket=accept(sock,(struct sockaddr*)NULL,NULL);

if(accept_socket<0)
{
printf("error in accepting\n");
exit(0);
}


/* read the message from client (number for which square root has to be found out */
read(accept_socket,msg,1000);

number =atof(msg);                                                           //convert the number to double type to find square root

if(number<0)
{
write(accept_socket,errmsg,1000);
}
else if(number>0)
{
result=sqrt(number);                                                        //find the square root
printf("Number recieved from client:%f\n",number);
printf("Square root of the number:%f\n",result);
sprintf(msg,"%f",result);                                                   //convert the square root back to char type in order to send it back to client
write(accept_socket,msg,strlen(msg)+1);                                     //write to client
}

else if(number==0)
{
result=sqrt(number);
printf("Number recieved from client:%f\n",number);
printf("Square root of the number:%f\n",result);
sprintf(msg,"%f",result);
write(accept_socket,msg,strlen(msg)+1);
}
else
{
write(accept_socket,errmsg1,1000);

}

close(accept_socket);
return 0;

}












