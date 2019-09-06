## _Computer Networks lab (CSC15206 : 2019:20) project_

# **Chat-server (using socket programming)**
It is a basic chat server programmed to handle multiple users (clients) using socket programming. You can download / upload files from / to the server and send messaages through the server to other connected clients.  
In a multi-client chat server, N clients are connected to a server and send messages. In this program, one of the clients send messages to the server and it(server) will sent back the messages to all other connected clients(we can say broadcast). The code is implemented using C language, with a TCP connection.  

## SERVER

Server takes incoming connections and ask for a username from every connection.


   

_server_log.txt keeps track of all the activities like connecting a new user,uploading file etc._


## CLIENT

Once the connection of socket is established with the server,the user is asked to input a username.

Every user is provided with five options :
1: Download file
2: Upload file
3: Send message
4: Disconnect from server
5: Close the server



The specifications of options available are:-
### 1.Download file
   The user is asked to input the name of the file.If the file exists with server the file is downloaded by the user otherwise an error message is shown.
   
### 2.Upload file
   The user is asked to input the name of the file which is to be uploaded.If the file does not exist with server,the file is    uploaded to the server.If the file already exists with the same name ,an option comes to overwrite the existing file with  the same name or abort the upload file process.     
   
### 3.Send message
   The user is asked to input message .The message is sent to all the remaining connected clients.
   
### 4.Disconnect from the server
   The user is disconnected from the server and the user leaves the chat server
   
### 5.Close the server
   Only an administrator which has the correct username and password can close the server for all users.To close the server     for all users, server first authenticates the username.If the username matches it asks for a password.If the password is       correct server gets closed for all the users otherwise an error message is displayed stating incorrect password.
