## _Computer Networks lab (CSC15206 : 2019:20) project_

# **Chat-server (using socket programming)**
It is a basic chat server programmed to handle multiple users (clients) using socket programming. You can download / upload files from / to the server and send messaages through the server to other connected clients.  
In a multi-client chat server, N clients are connected to a server and send messages. In this program, one of the clients send messages to the server and it(server) will sent back the messages to all other connected clients(we can say broadcast). The code is implemented using C language, with a TCP connection.  

![Client Server Interaction](http://vidyakv.files.wordpress.com/2011/12/cs-120-3-3341.png)

## SERVER
In the server program, first is the establishment of connection to a port.Server takes incoming connections and ask for a username from every connection.
The server stores the username of administrator and a password for authentication.Only an administrator can close the server.

In the case of a server, it wants to listen for incoming connections as well as keep reading from the connections it already have. select() gives the power to monitor several sockets at the same time. It’ll tell you which ones are ready for reading, which are ready for writing, and which sockets have raised exceptions.

int select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

If we want to see if we can read from standard input and some socket descriptor, sockfd, just add the file descriptors 0 and sockfd to the set readfds. The parameter numfds should be set to the values of the highest file descriptor plus one. When select() returns, readfds will be modified to reflect which of the file descriptors we

selected which is ready for reading. After ‘select’, it run through the existing connections looking for data to read. If we got one, new connections are handled and the new file descriptor is added to the master set by keeping track of the maximum file descriptor. If there is no need to handle new connection, handle data from a client. If there is any data in the recv_buf, it can be received by using recv().Or , the data is send to all other clients by using the function send().

Some features of select() command
*Select command allows to monitor multiple file descriptors, waiting until one of the file descriptors become active.
For example, if there is some data to be read on one of the sockets select will provide that information.
*Select works like an interrupt handler, which gets activated as soon as any file descriptor sends any data.

The server gives 5 options to its users:
*Download file
*Upload file
*Send message
*Disconnect from server
*Close the server

_server_log.txt keeps track of all the activities like connecting a new user,uploading file etc._


## CLIENT

Once the connection of socket is established with the server,the user is asked to input a username.

Every user is provided with five options :
1: Download file
2: Upload file
3: Send message
0 Disconnect from server
5: Close the server



The specifications of options available are:-
### 1.Download file
   The user is asked to input the name of the file.If the file exists with server the file is downloaded by the user otherwise an error message is shown.
   
### 2.Upload file
   The user is asked to input the name of the file which is to be uploaded.If the file does not exist with server,the file is    uploaded to the server.If the file already exists with the same name ,an option comes to overwrite the existing file with  the same name or abort the upload file process.     
   
### 3.Send message
   The user is asked to input message .The message is sent to all the remaining connected clients.
   
### 0:isconnect from the server
   The user is disconnected from the server and the user leaves the chat server
   
### 5.Close the server
   Only an administrator which has the correct username and password can close the server for all users.To close the server     for all users, server first authenticates the username.If the username matches it asks for a password.If the password is       correct server gets closed for all the users otherwise an error message is displayed stating incorrect password.
