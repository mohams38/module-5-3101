
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAX_CONTENT 255

int main(int argc, char const *arg[])
{
    // create server socket
    int serverSocketID = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketID < 0)
    {
        printf("Error: Could not create socket.\n");
        return 1;
    }

    // define server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9001);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // bind socket to the specified port and IP
    if (bind(serverSocketID, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Error: Bind failed.\n");
        close(serverSocketID);
        return 1;
    }

    printf("Listening for Connections\n");

    // listen for connections
    listen(serverSocketID, 1);

    // set infinite loop for server to accept connections from clients
    while (1)
    {
        printf("Waiting for next request from Client\n");

        // define variable to hold client socket
        int clientSocket = accept(serverSocketID, NULL, NULL);
        if (clientSocket < 0)
        {
            printf("Error: Could not accept client.\n");
            continue;
        }

        char fileName[256];
        char response[256];
        memset(fileName, 0, sizeof(fileName));
        memset(response, 0, sizeof(response));

        // receive filename from client
        recv(clientSocket, fileName, sizeof(fileName), 0);

        // remove newline from input
        fileName[strcspn(fileName, "\r\n")] = 0;

        // validate filename
        if (strlen(fileName) == 0 || strstr(fileName, "..") != NULL)
        {
            strcpy(response, "ERROR: Invalid file name.");
            send(clientSocket, response, strlen(response) + 1, 0);
            close(clientSocket);
            continue;
        }

        FILE *fp = fopen(fileName, "r");
        if (fp == NULL)
        {
            strcpy(response, "ERROR: File not found.");
            send(clientSocket, response, strlen(response) + 1, 0);
            close(clientSocket);
            continue;
        }
        // read up to 256 chars so we can detect overflow >255
        char fileContent[257];
        int count = fread(fileContent, 1, sizeof(fileContent), fp);

        if (count > MAX_CONTENT)
        {
            fclose(fp);
            strcpy(response, "ERROR: File size exceeds the allowed 255 characters.");
            send(clientSocket, response, strlen(response) + 1, 0);
            close(clientSocket);
            continue;
        }

        // extra check for files larger than 255 chars
        int extraChar = fgetc(fp);
        fclose(fp);

        if (extraChar != EOF)
        {
            strcpy(response, "ERROR: File size exceeds the allowed 255 characters.");
            send(clientSocket, response, strlen(response) + 1, 0);
            close(clientSocket);
            continue;
        }

        fileContent[count] = '\0';
        // send file content back to client
        send(clientSocket, fileContent, strlen(fileContent) + 1, 0);

        printf("Responded to client request for file: %s\n", fileName);
        close(clientSocket);
    }

    close(serverSocketID);
    return 0;
}
