/*
    Create a TCP socket
*/
 
#include<stdio.h>
#include<winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    int recv_size;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) //initialize winsock
    {
        printf("Failed. Error Code : %d",WSAGetLastError()); //prints out when initialize fails
        return 1;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) //creates socket
    {
        printf("Could not create socket : %d" , WSAGetLastError()); //prints out when creation fails
    }
 
    printf("Socket created.\n");
     
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");// 127.0.0.1 is local ip address
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 ); // make sure u have same port number as server
 
    //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0) //using connect' function from winsock to remote server
    {
        puts("connect error");
        return 1;
    }
     
    puts("Connected");
     
    //Send some data(test if clinet can send the msg to server)
    message = "SUGAR MAMA SUGAR PAPA SUGAR KID";//works with any character
    //message = "shorter string"; //test case
    if( send(s , message , strlen(message) , 0) < 0) //send message
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Sending\n");
	printf("Client is Sending : ");
	puts(message);
     
    //Receive a reply from the server
    if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
     
    puts("Reply received\n");
 
    //Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);
	system("pause");
    return 0;
}
