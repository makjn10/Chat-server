## _Computer Networks lab (CSC15206 : 2019:20) project_

# **Chat-server (using socket programming)**
It is a basic chat server programmed to handle multiple users (clients) using socket programming. You can download / upload files from / to the server and send messaages through the server to other connected clients.  
In a multi-client chat server, N clients are connected to a server and send messages. In this program, one of the clients send messages to the server and it(server) will sent back the messages to all other connected clients(we can say broadcast). The code is implemented using C language, with a TCP connection.  

## SERVER

Server takes incoming connections and ask for a username from every connection.
Every user is provided with four options :
1: Download file
2: Upload file
3: Send message
4: Exit the chat box

The specifications of options available are:-
### 1.Download file
   The user is asked to input the name of the file.If the file exists with server the file is downloaded by the user otherwise    an error message is shown.
   
### 2.Upload file
   The user is asked to input the name of the file which is to be uploaded.If the file does not exist with server,the file is      uploaded to the server.If the file already exists with the same name ,an option comes to overwrite the existing file with      the same name or abort the upload file process.
   
### 3.Send message
   The user is asked to input message .The message is sent to all the remaining connected clients.
   
### 4.Exit the chat box
   The user is disconnected from the server.

_server_log.txt keeps track of all the activities like connecting a new user,uploading file etc._


## CLIENT
