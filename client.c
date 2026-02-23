
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

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
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    int connectStatus = connect(serverSocketID, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (connectStatus == -1)
    {
        printf("Error: Could not connect to the server. \n");
    }
    else
    {
        char fileName[256];
        char serverMsg[256];

        printf("Enter filename to request: ");
        if (fgets(fileName, sizeof(fileName), stdin) == NULL)
        {
            printf("Error: Could not read input.\n");
            close(serverSocketID);
            return 1;
        }

        // send filename to server
        send(serverSocketID, fileName, strlen(fileName) + 1, 0);

        // receive response from server
        memset(serverMsg, 0, sizeof(serverMsg));
        recv(serverSocketID, serverMsg, sizeof(serverMsg), 0);

        printf("Response from Server: %s\n", serverMsg);
    }

    close(serverSocketID);
    return 0;
}
