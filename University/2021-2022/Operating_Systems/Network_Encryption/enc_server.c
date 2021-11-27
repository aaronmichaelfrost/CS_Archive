///
///		enc_server.c
///		Encryption server. Made to use with an enc_client. 
///     USAGE: enc_server port &
/// 
///		Aaron Frost - 11/26/2021
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char* msg) {
	perror(msg);
	exit(1);
}

/// <summary>
/// Maps the uppercase alphabet ASCII 65-90 to 0-25 where 26 maps to the space character
/// </summary>
char int_to_char(int x) {
    if (x == 26) {
        return ' ';
    }
    return x + 65;
}

/// <summary>
/// Maps the uppercase alphabet ASCII 65-90 to 0-25 where 26 maps to the space character
/// </summary>
char char_to_int(char ch) {
    if (ch == ' ') {
        return 26;
    }
    return ch - 65;
}

/// <summary>
/// Every character in message will be replaced with (message + key) % 27
/// </summary>
/// <param name="message">String to encrypt and output</param>
/// <param name="key">Secret key to encrypt message</param>
void encrypt(char* message, char* key) {
    int i;

    int len = strlen(message);
    for (i = 0; i < len; i++)
    {
        // Add together key and message based at 0, then get the remainder from 27
        int temp = (char_to_int(message[i]) + char_to_int(key[i])) % 27;
        message[i] = int_to_char(temp);
    }
    message[i] = '\0';
}

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address,
	int portNumber) {

	// Clear out the address struct
	memset((char*)address, '\0', sizeof(*address));

	// The address should be network capable
	address->sin_family = AF_INET;
	// Store the port number
	address->sin_port = htons(portNumber);
	// Allow a client at any address to connect to this server
	address->sin_addr.s_addr = INADDR_ANY;
}

int main(int argc, char* argv[]) {
    int connectionSocket, charsRead;
    char buffer[1000];
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t sizeOfClientInfo = sizeof(clientAddress);

    pid_t pid;

    // Check usage & args
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s port\n", argv[0]);
        exit(1);
    }

    // Create the socket that will listen for connections
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        error("ERROR opening socket");
    }

    // Set up the address struct for the server socket
    setupAddressStruct(&serverAddress, atoi(argv[1]));

    // Associate the socket to the port
    if (bind(listenSocket,
        (struct sockaddr*)&serverAddress,
        sizeof(serverAddress)) < 0) {
        error("ERROR on binding");
    }

    // Start listening for connetions. Allow up to 5 connections to queue up
    listen(listenSocket, 5);

    // Accept a connection, blocking if one is not available until one connects
    while (1) {
        // Accept the connection request which creates a connection socket
        connectionSocket = accept(listenSocket,
            (struct sockaddr*)&clientAddress,
            &sizeOfClientInfo);
        if (connectionSocket < 0) {
            error("ERROR on accept");
        }

        // Create child process to perform encryption
        pid = fork();

        switch (pid) {
            case -1: {
                error("Hull Breach. Failed to fork\n");
            }
                
            case 0: {
                // Child process

                char buffer[1000];
                char plaintext[100000];
                char key[100000];
                int characters_written = 0;
                int characters_read = 0;
                int message_size = 0;

                memset(buffer, '\0', sizeof(buffer));

                // Client should send the mode string "e" for encrypt
                while (characters_read == 0) {
                    characters_read = recv(connectionSocket, buffer, sizeof(buffer) - 1, 0);
                }
                if (characters_read < 0) {
                    error("ERROR reading from socket");
                }

                // If the client wants to decrypt, they are connected to the wrong server
                if (strcmp(buffer, "e") != 0) {
                    send(connectionSocket, "n", 1, 0);
                    exit(2);
                }

                // Tell client they connected to the right server
                send(connectionSocket, "y", 1, 0);


                // client should now send the size of the plaintext + newline + key
                memset(buffer, '\0', sizeof(buffer));
                characters_read = 0;
                while (characters_read == 0) {
                    characters_read = recv(connectionSocket, buffer, sizeof(buffer) - 1, 0);
                }
                if (characters_read < 0) {
                    error("ERROR reading from socket");
                }

                message_size = atoi(buffer);

                // Send ready message
                send(connectionSocket, "r", 1, 0);

                // Read plaintext + newline + key
                char recieved_message[message_size];
                memset(recieved_message, '\0', sizeof(recieved_message));
                charsRead = 0;
                while (charsRead < message_size) {
                    memset(buffer, '\0', sizeof(buffer));
                    charsRead += recv(connectionSocket, buffer, sizeof(buffer) - 1, 0);
                    strcat(recieved_message, buffer);
                }

                // Copy over the plaintext into the plaintext buffer
                int i = 0;
                while (recieved_message[i] != '\n') {
                    plaintext[i] = recieved_message[i];
                    i++;
                }

                plaintext[i] = '\0';

                // Include newline in the plaintext
                i++;

                int j = 0;

                // Copy over the key into the key buffer
                while (i < message_size) {
                    key[j++] = recieved_message[i++];
                }

                // Encrypt plaintext
                encrypt(plaintext, key);

                // Send ciphertext to client
                send(connectionSocket, plaintext, sizeof(plaintext) - 1, 0);
            }
        }

        close(connectionSocket);
    }
    // Close the listening socket
    close(listenSocket);
    return 0;
}