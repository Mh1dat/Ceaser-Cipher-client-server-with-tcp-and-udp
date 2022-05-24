# Ceaser-Cipher-Server-Program-Responds-to-both-TCP-and-UDP-Requests
The server will accept requests on the same port (whether TCP or UDP), which is passed as an argument in the command line when starting the server.
Therefore, I/O multiplexing is needed to enhance the program responsiveness.
Server-Side:
The server implements an encryption/decryption service using ceaser cipher with key=3.
When a TCP request is received, a child server will reply accordingly to that request (The TCP part is concurrent). When a UDP request is received, the server itself (the UDP part is iterative) 
will reply to request accordingly. The server can encrypt or decrypt the received text message based on the first 
character. If the first character is e, the server encrypts the received message, converts it to capital-letter, and 
sends the reply back to the client. If the first character is d, the server decrypt the received message, converts it 
to capital-letter, and sends the reply back to the client. Otherwise, an error message is sent back to the client 
with an appropriate message. 
Client-Side:
To test my server, I need to implement two clients, one TCP and one UDP. Both will display the 
following menu to the user to select from:
1. Encrypt a message
2. Decrypt a message
If the user selects 1, the client asks the user to enter a null-terminated string representing the plaintext 
message, sends the message to the server, and waits for the reply which is printed out on the standard output 
once received. If the user selects 2, the client asks the user to enter a null-terminated string representing the 
ciphertext message, sends the message to the server, and waits for reply which is printed out on the standard
output once received. The client, then, redisplay the menu for the user to input a new selection and continue 
doing so until the user enters the signal SIGQUIT, at which time the client prints “done” on the standard output 
and both the client and the corresponding server exits
The client should use command line arguments to read the server hostname (not IP address ) and port 
number of the server, and leave it for the kernel to assign its local socket address. To do so, I edited 
the /etc/hosts file in my local machine and add a line that specifies the server name as nes416_server
and assign it the IP address of my machine.
