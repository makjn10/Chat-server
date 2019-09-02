void send_to_all(int other_sockets , int client_socket , int server_socket , int nbytes_recvd , char * buffer , fd_set * master){
	if(FD_ISSET(other-sockets , master)){
		if(other_sockets != server_socket && other_sockets != client_socket) {
			if(send(other_sockets , buffer , nbytes_recvd , 0) == -1){
				printf("Message sending error to socket %d\n" , other_sockets);
			}
		}
	}
	return;
}
		
void send_recv_msg(int client_socket , fd_set * master , int server_socket , int fdmax , char username[]){
	int nbytes_recvd , other_sockets; //number of bytes recieved and descriptor for other sockets in master set
	char buffer[1024]; //buffer used for transfer
	
	//log file operations
	FILE * log_file = fopen("server_log.txt" , "a");
	time_t TIME;
	struct tm tm;

	if((nbytes_recvd = recv(client_socket, buffer , sizeof(buffer) , 0)) <= 0){ // if user disconnected or some error
		int status = -1; //to tell if client disconnected with error(-1) or something else
		if(nbytes_recvd == 0){
			status = 1; //client disconnected with no error
			printf("User : %s (socket %d) disconnected from the server.\n", username , client_socket);
		}else{
			printf("Recieving error from client...\nClosing connection...\nUser : %s (socket %d) disconnected from the server.\n" , username , client_socket)
		}
		close(client_socket);//closing client socket
		FD_CLR(client_socket, master);//removing client_socket from the master set

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
	else{ // recieve buffer from the client and broad cast to the others in the master set
		printf("User : %s sending message to all the connected users...\n" , username);
		for(other_sockets = 0; other_sockets <= fdmax; other_sockets++){
			send_to_all(other_sockets , client_socket , server_socket, nbytes_recvd , buffer , master);
		}
		//logging to log file at server
		TIME = time(NULL);
		tm = *localtime(&TIME);
		printf("Logging operation...\n");
		fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
		fprintf(log_file , "User : %s\n   sent a message : %s\n" , username , buffer);
		printf("Logged successfully\n");
		printf("Message sent successfully...\n");
	}
	fclose(log_file);
	return;
}