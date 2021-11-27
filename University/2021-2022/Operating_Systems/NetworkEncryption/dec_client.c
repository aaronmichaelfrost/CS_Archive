///
///		dec_client.c
///		Decryption client. Pass in ciphertext and key. This client asks the enc_server to decrypt the message. 
///     First you must run dec_server and use the same port
///     USAGE: dec_client ciphertext.txt key.txt port
/// 
///		Aaron Frost - 11/26/2021
///

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

#define HOSTNAME "localhost"

// Error function used for reporting issues
void error(const char* msg) {
    perror(msg);
    exit(1);
}

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address,
    int portNumber,
    char* hostname) {

    // Clear out the address struct
    memset((char*)address, '\0', sizeof(*address));

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);

    // Get the DNS entry for this host name
    struct hostent* hostInfo = gethostbyname(hostname);
    if (hostInfo == NULL) {
        fprintf(stderr, "CLIENT: ERROR, no such host\n");
        exit(0);
    }
    // Copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char*)&address->sin_addr.s_addr,
        hostInfo->h_addr_list[0],
        hostInfo->h_length);
}

int main(int argc, char* argv[]) {

    // Check usage and args
    if (argc < 4) {
        fprintf(stderr, "USAGE: %s ciphertext key port\n", argv[0]);
        exit(0);
    }

    char* cipher;
    char* key;

    long cipher_size;
    long key_size;

    // Read plaintext into a string
    FILE* cipher_fp = fopen(argv[1], "r");
    if (cipher_fp == NULL) {
        fprintf(stderr, "CLIENT: ERROR: cipher file failed to open\n");
        exit(0);
    }
    // Get the number of bytes minus newline char
    fseek(cipher_fp, 0L, SEEK_END);
    cipher_size = ftell(cipher_fp) - 1;
    // Reset the file position indicator to beginning of file
    fseek(cipher_fp, 0L, SEEK_SET);
    cipher = (char*)calloc(cipher_size, sizeof(char));
    // Copy file text into buffer
    fread(cipher, sizeof(char), cipher_size, cipher_fp);
    fclose(cipher_fp);


    // Read key into string
    FILE* key_fp = fopen(argv[2], "r");
    if (key_fp == NULL) {
        fprintf(stderr, "CLIENT: ERROR: Key file failed to open\n");
        exit(0);
    }
    // Get the number of bytes minus newline char
    fseek(key_fp, 0L, SEEK_END);
    key_size = ftell(key_fp) - 1;
    // Reset the file position indicator to beginning of file
    fseek(key_fp, 0L, SEEK_SET);
    key = (char*)calloc(key_size, sizeof(char));
    // Copy file text into buffer
    fread(key, sizeof(char), key_size, key_fp);
    fclose(key_fp);


    // Compare length of plaintext and key
    if (key_size < cipher_size) {
        fprintf(stderr, "CLIENT: ERROR: Key size %d < plaintext size %d\n", key_size, cipher_size);
        exit(1);
    }

    // Check characters in plaintext
    for (size_t i = 0; i < cipher_size; i++)
    {
        if (cipher[i] > 90 || (cipher[i] < 65 && cipher[i] != 32)) {
            fprintf(stderr, "CLIENT: ERROR: invalid character in plaintext\n");
            exit(1);
        }
    }

    // Check characters in key
    for (size_t i = 0; i < key_size; i++)
    {
        if (key[i] > 90 || (key[i] < 65 && key[i] != 32)) {
            fprintf(stderr, "CLIENT: ERROR: invalid character in key\n");
            exit(1);
        }
    }

    // Get the port
    int port = atoi(argv[3]);

    // Connect to enc_server
    int socketFD, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    char buffer[1000];

    // Create a socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        error("CLIENT: ERROR opening socket");
    }

    // Set up the server address struct
    setupAddressStruct(&serverAddress, port, HOSTNAME);

    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        error("CLIENT: ERROR connecting");
    }


    // Make sure we are connecting to the correct server.

    // Send an "d" for dcryption
    char* message = "d";
    charsWritten = send(socketFD, message, strlen(message), 0);
    if (charsWritten < 0) {
        error("CLIENT: ERROR writing to socket");
    }

    // If we got back a "y" for yes, we are connected to the correct server, else we got an "n" so throw an error and return
    charsRead = 0;
    while (charsRead == 0) {
        charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
    }
    if (charsRead < 0) {
        error("CLIENT: ERROR reading from socket");
    }
    if (strcmp(buffer, "n") == 0) {
        fprintf(stderr, "CLIENT: ERROR incorrect server\n");
        exit(2);
    }

    if (strcmp(buffer, "y") != 0) {
        fprintf(stderr, "CLIENT: ERROR incorrect server\n");
        exit(2);
    }

    printf("Decryption client got a y!\n");


    // SEND MESSAGE SIZE
    int full_message_size = cipher_size + key_size + 1;

    memset(buffer, '\0', sizeof(buffer));
    sprintf(buffer, "%d", full_message_size);

    charsWritten = send(socketFD, buffer, strlen(buffer), 0);
    if (charsWritten < 0) {
        error("CLIENT: ERROR writing to socket");
    }
    if (charsWritten < strlen(buffer)) {
        printf("CLIENT: WARNING: Not all data written to socket!\n");
    }


    // WAIT FOR READY MESSAGE FROM SERVER
    memset(buffer, '\0', sizeof(buffer));
    charsRead = 0;
    while (charsRead == 0) {
        charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
    }
    if (charsRead < 0) {
        error("CLIENT: ERROR reading from socket");
    }
    if (strcmp(buffer, "r") != 0) {
        fprintf(stderr, "CLIENT: did not recieve ready message, instead recieved %s\n", buffer);
        exit(2);
    }

    // SEND PLAINTEXT + KEY
    memset(buffer, '\0', sizeof(buffer));

    char full_message[full_message_size];
    // Concatenate plaintext and key into buffer delimited with newline
    sprintf(full_message, "%s\n%s", cipher, key);

    while (charsWritten < full_message_size) {
        charsWritten += send(socketFD, full_message, strlen(full_message), 0);
    }

    // RECIEVE CIPHERTEXT FROM SERVER
    char recieved_plaintext[100000];
    charsRead = 0;

    // We expect the ciphertext to be the same length as the plaintext
    while (charsRead < cipher_size) {
        memset(buffer, '\0', sizeof(buffer));
        charsRead += recv(socketFD, buffer, sizeof(buffer) - 1, 0);
        strcat(recieved_plaintext, buffer);
    }

    // Output ciphertext
    printf("%s\n", recieved_plaintext);

    free(cipher);

    // Close the socket
    close(socketFD);
    return 0;
}