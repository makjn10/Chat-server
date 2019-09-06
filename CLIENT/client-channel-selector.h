void send_recv(int client_socket , int sock , char UNAME[]){
	
	char username[100];
	strcpy(username , UNAME);

	char option[2];//select the option of what to do
	option[0] = '\0';
	scanf("%s" , option);
	while ((getchar()) != '\n'); //clearing buffer
	//sending option to server
	send(client_socket , option , sizeof(option) , 0);

	char filename[100]; //filename for download and upload operations
	send(client_socket , username , sizeof(username) , 0);

	if(option[0] == '1'){
		//DOWNLOAD A FILE FROM SERVER
		printf("    Enter file name : ");
		fgets(filename , 100 , stdin);//getting the file name
		
		filename[strlen(filename) - 1] = '\0';	//removing newline char at last from file name
		
		send(client_socket , filename , sizeof(filename) , 0);//sending filename
		getFile(filename , client_socket);
	}

	else if(option[0] == '2'){
		//UPLOAD A FILE TO SERVER
		printf("    Enter file name : ");
		fgets(filename , 100 , stdin);

		filename[strlen(filename) - 1] = '\0';	//removing newline char at last from file name

		send(client_socket , filename , sizeof(filename) , 0);//sending filename
		sendFile(filename , client_socket);
	}

	else if(option[0] == '3'){
		send_recv_msg(client_socket , sock);
	}

	else if(option[0] == '4'){
		printf("    SERVER CLOSED\n\n");
		exit(0);
	}

	else{
		printf("    Disconnected from the server!...Exiting...\n\n");
		close(client_socket);
		exit(0);
	}
	return;
}