void sendFile(char filename[] , int client_socket){

	int status; //tells if file already present at server with same name or its fine to upload the file
	//-1 : File already present
	// 1 : All Clear

	recv(client_socket , &status , sizeof(status) , 0);

	//file pointer
	FILE * ptr = fopen(filename , "r");
	if(ptr == NULL){
		int abort = 1;
		send(client_socket , &abort , sizeof(abort) , 0);//sending option to server
		printf("    No such file present...Aborting transfer...\n    File tranfer aborted successfully.\n");	
		return;
	}

	if(status == -1){
		//FILE WITH SAME NAME PRESENT AT SERVER
		int option; // tell what user wants to do

		printf("    File with same name already present at server...\n    Do you want to overwrite the file or abort?\n    Enter option (0 : Overwrite or 1/any number : Abort) : ");
		fflush(stdout);
		scanf("%d" , &option);
		while ((getchar()) != '\n'); //clearing buffer

		send(client_socket , &option , sizeof(option) , 0);//sending option to server

		if(option == 0){
			//OVERWRITE
			char buffer[1024]; //buffer used for transfer
			int flag = 1; // used as a flag for transfer

			//TRANSFER START
			printf("\n    Tranferring file \"%s\" to the server...\n" , filename);
			while(fgets(buffer , 1024 , ptr) != NULL){
				send(client_socket , &flag , sizeof(flag) , 0);
				send(client_socket , buffer , sizeof(buffer) , 0);
			}
			flag = 0;
			send(client_socket , &flag , sizeof(flag) , 0);
			printf("    File \"%s\" uploaded to the server successfully\n" , filename);

			fclose(ptr);
			return;
		}
		else{
			//ABORT
			printf("s\n    File transfer aborted successfully.\n");
			return;
		}
	}
	else{
		//TRANSFER NORMALLY
		char buffer[1024]; //buffer used for transfer
		int flag = 1; // used as a flag for transfer
		
		//TRANSFER START
		printf("\n    Tranferring file \"%s\" to the server...\n" , filename);
		while(fgets(buffer , 1024 , ptr) != NULL){
			send(client_socket , &flag , sizeof(flag) , 0);
			send(client_socket , buffer , sizeof(buffer) , 0);
		}
		flag = 0;
		send(client_socket , &flag , sizeof(flag) , 0);
		printf("    File \"%s\" uploaded to the server successfully\n" , filename);
		fclose(ptr);
		return;
	}
}