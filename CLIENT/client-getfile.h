void getFile(char filename[] , int client_socket){

	int option; // to know the transfer status
	// -1 : no such file present at server side
	//  1 : transferring
	//  0 : tranfer stop

	recv(client_socket , &option , sizeof(option) , 0); // recieve status

	if(option == -1){
		//FILE NOT PRESENT AT SERVER
		printf("    No such file present at server.\n");
		return;
	}
	else{
		//TRANSFER START
		//file pointer
		FILE * ptr = fopen(filename , "w");

		char buffer[1024]; //buffer for transfer

		printf("    Downloading file \"%s\" from server...\n" , filename);
		while(option == 1){
			recv(client_socket , buffer , sizeof(buffer) , 0);
			fprintf(ptr , "%s" , buffer);
			recv(client_socket , &option , sizeof(option) , 0);
		}
		printf("    File \"%s\" downloaded successfully.\n" , filename);
		fclose(ptr);
		//TRANSFER COMPLETE
		return;
	}
}
