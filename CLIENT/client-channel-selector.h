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

	int password;

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
		int access = 0 , askpass = 0 , pass;
		recv(client_socket , &askpass , sizeof(askpass) , 0);
		if(askpass == 1){
			//ASKING FOR PASSWORD
			printf("    Enter password : ");
			fflush(stdout);
			scanf("%d" , &pass);
			while ((getchar()) != '\n'); //clearing buffer

			send(client_socket , &pass , sizeof(pass) , 0);
			recv(client_socket , &access , sizeof(access) , 0);

			if(access == 0){
				//ACCESS DENIED
				printf("    Invalid password. Access Denied.\n");
				return;
			}
			else{
				//ACCESS GRANTED
				printf("    SERVER CLOSED\n\n");
				exit(0);
			}

		}
		else{
			//NOT AN ADMIN USER
			printf("    You are not an admin user...\n    Please contact the admin for this operation.\n");
			return;
		}
	}

	else if(option[0] == '0'){
		printf("    Disconnected from the server!...Exiting...\n\n");
		close(client_socket);
		exit(0);
	}
	else{
		printf("    Invalid option... Please see options carefully and enter.\n");
	}
	return;
}
