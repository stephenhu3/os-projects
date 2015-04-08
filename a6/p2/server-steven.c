/*
    Live Server on port 8888
*/
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c,recv_size;
	char *message , client_reply[2000];

 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
     
    puts("Bind done");
 
    //Listen to incoming connections
    listen(s , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
     
    c = sizeof(struct sockaddr_in);
     
    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
        puts("Connection accepted");
         
        //Reply to the client
        message = "This is server, looking for client!! Houston are you there ??!! \n";
        send(new_socket , message , strlen(message) , 0);
		//Receive a reply from the server
    if((recv_size = recv(new_socket, client_reply , 2000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
	else
	{
	printf("I have receive your messege \n");
	printf("Reply from client : ");
	client_reply[recv_size] = '\0';
	puts(client_reply);
	}


    }
     
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
        return 1;
    }
 
    closesocket(s);
    WSACleanup();
     
    return 0;
}