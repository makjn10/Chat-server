#include "server.h"

#define PORT 8001

int main(){
	//time structures for log operations
	time_t TIME;
	struct tm tm;
	//log file
	FILE * log_file = fopen("server_log.txt" , "a");

	//creating server socket
	int server_socket;
	if((server_socket = socket(AF_INET , SOCK_STREAM , 0)) == -1){
		printf("Server socket error!\nServer closing...\n");
		return 0;
	}

	//specifying server address details
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	bzero(&server_address.sin_zero,0);

	//binding the server socket to server address and port
	if(bind(server_socket , (struct sockaddr *) &server_address , sizeof(server_address)) == -1){
		printf("Server binding error!\nServer closing...\n");
		close(server_socket);
		return 0;	
	}

	//listening on the port for clients;
	if(listen(server_socket , 10) == -1){
		printf("Server listen error!\nServer closing...\n");
		close(server_socket);
		return 0;	
	}

	printf("SERVER STARTED : waiting for clients at port %d...\n" , PORT);
	TIME = time(NULL);
	tm = *localtime(&TIME);
	
	fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
	fprintf(log_file , "---- SERVER STARTED ----\n");

	//server socket created
	//server address declared
	//server address binded to the socket
	//server socket has started listening

	//-------------------------------------------------------------------------------------------//

	//server data send and recieve to the client
	//storing data in FILES at server side


	//commands input from client to process 
	char username[100];

	//using select function to deal with multiple clients
	fd_set master , read_fds; // to hold all the socket descriptors and all the active client descriptors respectively
	int fdmax , server_status = 1;
	//server status = 1 : running
	//server status = 0 : closed due to error
	//Server status = -1 : Closed normally

	int client_socket;
	struct sockaddr_in client_address;
	//initializing the sets
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	FD_SET(server_socket , &master);
	fdmax = server_socket;

	//running the server indefinitely
	while(1){
		read_fds = master;//creating a copy of master set as select is destructive
		if(select(fdmax + 1 , &read_fds , NULL , NULL , NULL) == -1){
			printf("Server select error... Closing the server...\n");
			break;
		}

		for(int socks = 0 ; socks <= fdmax ; socks++){
			if(FD_ISSET(socks , &read_fds)){
				//CONNECT TO THE CLIENT (CLIENT INCOMING)
				if(socks == server_socket){
					//it is the server socket
					//accepting the connections

					socklen_t addrlen;
					addrlen = sizeof(struct sockaddr_in);
					client_socket = accept(server_socket , (struct sockaddr *) &client_address , &addrlen);
					if(client_socket == -1){
						printf("Server accept error...Closing the server...\n");
						server_status = 0;
						break;
					}
					else{
						FD_SET(client_socket , &master);//adding to the master set
						if(client_socket > fdmax){
							fdmax = client_socket;
						}
						printf("New connection from %s on port %d \n", inet_ntoa(client_address.sin_addr) , ntohs(client_address.sin_port));
						//logging to log file
						TIME = time(NULL);
						tm = *localtime(&TIME);
	
						fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
						fprintf(log_file , "New connection from %s on port %d \n", inet_ntoa(client_address.sin_addr) , ntohs(client_address.sin_port));

						//sending welcome string to the client
						char welcomeMsg[1024] = "Welcome to the BTECH CSE Chat Server!\r\n";
						send(client_socket , welcomeMsg , sizeof(welcomeMsg) , 0);
					}
				}

				else{
					//it is a client socket
					//send and recv of data
					int option , nbytes_recvd; //to know what client wants to do
					//get(1) - to download file from server - option 1
					//put(2) - to upload file to server - option 2
					//msg(3) - to send a message to server and thus the clients - option 3
					//quit(4) - close the server
					client_socket = socks;

					nbytes_recvd = recv(client_socket , &option , sizeof(option) , 0);
					
					//checking if client disconnected 
					if(nbytes_recvd <= 0){
						int status = -1; //to tell if client disconnected with error(-1) or something else
						if(nbytes_recvd == 0){
							status = 1; //client disconnected with no error
							printf("User : %s (socket %d) disconnected from the server.\n", username , client_socket);
						}
						else if(nbytes_recvd < 0){
							printf("Recieving error from client...\nClosing connection...\nUser : %s (socket %d) disconnected from the server.\n" , username , client_socket);
						}
						close(client_socket);//closing client socket
						FD_CLR(client_socket, &master);//removing client_socket from the master set

						//logging to log file at server side
						TIME = time(NULL);
						tm = *localtime(&TIME);
						printf("Logging operation...\n");
						fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
						if(status == -1){
							fprintf(log_file , "User : %s\n   Disconnected...\n" , username);
						}
						else{
							fprintf(log_file , "User : %s\n   Disconnected...(due to recieving error)\n" , username);
						}
						printf("Logged successfully\n");
					}

					else{
						//continue with data send/recv
						//recieving username
						recv(client_socket , username , sizeof(username) , 0);

						//calling functions as per the option
						if(option == 1){
							//SEND FILE TO THE CLIENT
							//get the file name
							char filename[100];
							recv(client_socket , filename , sizeof(filename) , 0);
							sendFile(filename , client_socket , username);
						}
						else if(option == 2){
							//GET FILE FROM THE CLIENT
							//get the filename
							char filename[100];
							recv(client_socket , filename , sizeof(filename) , 0);
							getFile(filename , client_socket , username);
						}
						else if(option == 3){
							//RECIEVE MSG FROM THE CIIENT AND BROADCAST TO THE OTHERS
							send_recv_msg(client_socket , &master , server_socket , fdmax , username);
						}
						else if(option == 4){
							//CLOSE THE SERVER
							server_status = -1;
							break;
						}
						else{
							printf("Invalid option");
							//NO OPERATION DONE
						}
					}					
				}
			}
		}
		if(server_status == 0 || server_status == -1){
			break;
		}
	}
	//logging server closing
	TIME = time(NULL);
	tm = *localtime(&TIME);
	printf("Server closing...\n");
	fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
	if(server_status == -1){
		fprintf(log_file , "---- SERVER CLOSING ----\n\n\n");
	}
	else{
		fprintf(log_file , "---- SERVER CLOSING(due to error) ----\n\n\n");
	}
	return 0;
}
