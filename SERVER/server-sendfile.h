void sendFile(char filename[] , int client_socket , char username[]){
	char buffer[1024]; //buffer for transferring
	int option = -1; // used as a flag for tranfer

	FILE * ptr = fopen(filename , "r");
	
	if(ptr == NULL){
		//FILE NOT PRESENT / FAILED TO OPEN
		send(client_socket , &option , sizeof(option) , 0);
		printf("Transfer failed as n such file present\n");
	}
	else{
		//TRANSFER
		printf("Transfering file : \"%s\" to User : %s ...\n" , filename , username);
		option = 1; //start transferring
	
		while(fgets(buffer , 1024 , ptr) != EOF){
			send(client_socket , &option , sizeof(option) , 0);
			send(client_socket , buffer , sizeof(buffer) , 0);
		}
	
		option = 0; //transfer complete
		send(client_socket , &option , sizeof(option) , 0);
		printf("Transfer completed successfully\n");
		fclose(ptr);
	}
	
	//adding log to log file
	printf("Logging operation...\n");
	FILE * log_file = fopen("server_log.txt" , "a");
	//adding time stamp to log file
	time_t TIME = time(NULL);
	struct tm tm = *localtime(&TIME);

	frintf(log_file , "%d/%d/%d %d:%d:%d :: ", tm.tm_mday, tm.tm_mon + 1 , tm.tm_year + 1900 , tm.tm_hour , tm.tm_min , tm.tm_sec);
	if(option == 0){
		fprintf(log_file , "User served : %s\n   Downloaded file : \"%s\"\n" , username , filename);
	}
	else{
		fprintf(log_file , "User served : %s\n   Download failed for file : \"%s\" as no such file present / file failed to open.\n" , username , filename);
	}
	printf("Logged successfully\n");
	fclose(log_file);
	return;
}
