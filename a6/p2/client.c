#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <pthread.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <time.h>
#include <ctype.h>
//#include <netinet/in.h>
#include <string.h>
#include <errno.h>
//#include <arpa/inet.h> 
// NEED TO CHANGE ALL THIS TO windwos

int tcp_connect(char *hostname[], int portnumber);
// connect to server running at tcp port 11261 on lulu.ugrad.cs.ubc.ca


 
int main(int argc, char **argv) 
{

	
	int port = atoi(argv[3]);
	
	tcp_connect(hostname[],port);


}







int tcp_connect (char *hostname[], int portnumber)
{
	char * servName = "lulu.ugard.cs.ubc.ca";
	

	char servIP[16]; // change to name --> ip
	struct hostent *hent;
	hent =gethostbyname(servName);
	inet_ntop(AF_INET,(void *)hent->h_addr_list[0],servIP,16);
	
	int rtnVal = inet_pton(AF_INET,servIP,&ServAddr.sin_addr.s_addr); //convert endiean
	if(rtnVal <= 0)
		dieMsg("inet fail",rtnVal);

	servAddr.sin_port = htons(servPort);
	servAddr.sin_family = AF_INET;
	printf("%s\n",servIP);


	long addres;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(11261); //connect to port 11261 of server
	
	int socket = socket(SF_INET, SOCK_STREAM, 0) //not sure 3rd argument shld be IPRPOTO_TCP or 0
	if(sock <0)printf("sock fail",sock); 

	if connect(sock,(struct sockaddr*) &servAddr,sizeof(servAddr))<0);
	{

		printf("con fail",-1);

	}	



struct sockaddr_in myaddr;
	 
	bind(clientsocket,myaddr,sizeof(addres);

	struct sockaddr_in uraddr;

	
	connect(clientsocket, (struct sockaddr*) uraddr



}
