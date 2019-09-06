#include "client.h"

#define PORT 8005

int main(){
	//creating client socket
	int client_socket = socket(AF_INET , SOCK_STREAM , 0);
	if(client_socket == -1){
		printf("> Socket creaion error at client side.\nExiting...\n");
		exit(1);
	}

	//defining server address
	struct  sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;
	bzero(&server_address.sin_zero,0);

	//getting the username
	char username[100];
	printf("\n> Enter your username : ");
	fgets(username , 100 , stdin);
	username[strlen(username) - 1] = '\0';//setting last char as end


	//connecting to server
	if(connect(client_socket , (struct sockaddr *) &server_address , sizeof(struct sockaddr)) == -1){
		printf("> Connecting error...Exiting...\n");
		exit(3);
	}
	else{
		char welcome_msg[1024];
		recv(client_socket , welcome_msg , sizeof(welcome_msg) , 0);
		printf("\n             %s\n", welcome_msg); //13 spaces
	}

	//creating fd sets
	fd_set master , read_fs;
	FD_ZERO(&master);
	FD_ZERO(&read_fs);
	FD_SET(0 , &master);
	FD_SET(client_socket , &master);

	//telling instructions
	printf("> INSTRUCTIONS : \nOptions :\n  * (1 : Download file)\n  * (2 : Upload file)\n  * (3 : Send message)\n  * (0 : Disconnnect)\n  * [4 : Close the server **(only for admin)]");
	printf("\n> Enter the option and then the required fields. \n\n> ");
	fflush(stdout);

	int fdmax = client_socket;
	int socks;
	while(1){
		read_fs = master;
		if(select(fdmax + 1 , &read_fs , NULL , NULL , NULL) == -1){
			printf("> Select error at client side...Exiting...\n");
			fflush(stdout);
			exit(4);
		}
		
		for(socks = 0 ; socks <= fdmax ; socks++){
			if(FD_ISSET(socks , &read_fs)){
				if(socks == 0)	send_recv(client_socket , socks , username);//call client-channel-selector
				else	send_recv_msg(client_socket , socks); //call client-message
			}
		}
		printf("\n> ");
		fflush(stdout);
	}

	printf("> Disconnected from server\n");
	close(client_socket);
	return 0;
}
