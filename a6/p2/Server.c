#include<stdio.h>
#include<winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c, recv_size;
    char *message, client_reply[2000];
 
    printf("\nInitialising Winsock.......");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){ //initialising winsock
        printf("Failed. Error Code : %d",WSAGetLastError()); //prints out if initialising fails
        return 1;
    }
     
    printf(" Initialised!\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) //create a socket
        printf("Could not create socket : %d" , WSAGetLastError()); //prints out if creation fails
 
    printf("Socket created!\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR) //bind socket
     {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE); // if bind fails, prints error msg and exit
    }
	else
	    puts("Bind done!");
     
 
    //Listen to incoming connections
    listen(s , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
     
    c = sizeof(struct sockaddr_in);

    //new_socket = accept(s , (struct sockaddr *)&client, &c); //accept connection from client
    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
		 puts("Connection accepted");
         
        //Reply to the client
        message = "This is server. Hello Client, I have received your connection\n";
        send(new_socket , message , strlen(message) , 0);
		//Receive a reply from the server
    if((recv_size = recv(new_socket, client_reply , 2000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
	else
	{
		printf("I have receive your message \n");
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
     
   
 
    //Reply to client(test if client can receive or not)
    //message = "Hello Client , I have received your connection. But I have to go now, Bye\n";
    //message = "shorter string"; //test case
    //send(new_socket , message , strlen(message) , 0); //send message to client that it has been connected
     
    //getchar();
 
    //without closesocket keep running client until it runs.
    closesocket(s);
    WSACleanup();
 
    //system("pause");
    return 0;
}
