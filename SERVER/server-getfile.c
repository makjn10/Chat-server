void Tranfer(int client_socket , char username[] , char filename[] , FILE * file){
	char buffer[1024]; //buffer for transferring
	int option = -1; // used as a flag for tranfer

	//TRANSFER START
	printf("Transferring file : \"%s\" from User : %s ...\n" , filename , username);
	recv(client_socket , &option , size(option) , 0);
	while(option == 1){
		recv(client_socket , buffer , sizeof(buffer) , 0);
		fprintf(file , "%s" , buffer);
		recv(client_socket , &option , size(option) , 0);
	}
	printf("Transfer completed successfully\n");
	//TRANSFER COMPLETE
}

void getFile(char filename[] , int client_socket , char username[]){
	int status = -1;
	// status : -1 : file already present
	// status : 1 : file can be uploaded
	int get_command;

	FILE * ptr; // pointer to the file

	//LOG FILE
	//TIME STRUCTURES to be used
	FILE * log_file = fopen("server_log.txt" , "a");
	//adding time stamp to log file
	time_t TIME;
	struct tm tm;

	//checking if already such a file exists or not
	FILE * check_if_present = fopen(filename , "r");
	if(check_if_present != NULL){
		//ALREADY A FILE PRESENT WITH SUCH NAME
		printf("FILE %s ALREADY PRESENT\n" , filename);
		printf("Awaiting command from the user...\n"); // await command if status == -1

		//closing check file
		fclose(check_if_present);

		send(client_socket , &status , sizeof(status) , 0);
		get_command = -1; //get the command to delete or rename the file
		// delete the present file (overwrite) : 0;
		// cancel the tranfer : -1
		recv(client_socket , &get_command , sizeof(get_command) , 0);

		//time for log file
		if(get_command == 0){
			// overwrite the file
			status = 1;
			ptr = fopen(filename , "w");
			printf("Overwriting the file : %s ...\n" , filename);

			//calling transfer
			Transfer(client_socket , username , filename , ptr);
			
			//time oprations for log file
			TIME = time(NULL);
			tm = *localtime(&TIME);

			printf("Logging operation...\n");
			fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
			fprintf(log_file , "User served : %s\n   Overwrote the file : %s\n" , username , filename);
			printf("Logged successfully\n");
			
			fclose(ptr);
			fclose(log_file);
			return;

		}
		else{
			//INVALID COMMAND - we choose to abort the transfer
			printf("Aborting the transfer...\n");
			//time oprations for log file
			TIME = time(NULL);
			tm = *localtime(&TIME);

			printf("Logging operation...\n");
			fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
			fprintf(log_file , "User served : %s\n   Transfer cancelled as a file already present with name %s\n" , username , filename);
			printf("Logged successfully\n");

			printf("Transfer aborted.\n");
			fclose(log_file);
			return;
		}

	}
	else{
		status = 1;
		send(client_socket , &status , sizeof(status) , 0);

		ptr = fopen(filename , "w");

		//calling transfer
		Transfer(client_socket , username , filename , ptr);
			
		//time oprations for log file
		TIME = time(NULL);
		tm = *localtime(&TIME);

		printf("Logging operation...\n");
		fprintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
		fprintf(log_file , "User served : %s\n   Uploaded the file : %s\n" , username , filename);
		printf("Logged successfully\n");
		
		fclose(ptr);
		fclose(log_file);
		return;
	}
}