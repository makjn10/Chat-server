void send_recv_msg(int client_socket , int i){
	char message[1024]; //recieve message or send message
	int nbyte_recvd;

	if(i == 0){
		//inputting message
		printf("    Enter message : ");
		fflush(stdout);
		fgets(message , 1024 , stdin);
		message[strlen(message) - 1] = '\0';
		send(client_socket , message , sizeof(message) , 0);	
	}
	else{
		nbyte_recvd = recv(client_socket , message , sizeof(message) , 0);
		//message[nbyte_recvd] = '\0';

		if(message[0] == '\\' && message[1] == '\\'){
			printf("Server closed by the Admin... Exiting...\n\n");
			exit(0);
		}
		printf("NEW MESSAGE from %s\n" , message);
		fflush(stdout);
	}
	return;
}
