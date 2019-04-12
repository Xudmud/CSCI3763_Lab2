#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 24601 //I am Jean Valjean!

int main(int argc, char const *argv[])
{
    int clientSocket, ret;
    struct sockaddr_in servAddr;
    /* Allocate a command buffer */
    char* buffer = calloc(1024,sizeof(char*));
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0)
    {
        fprintf(stderr,"Connection error: Failed to crate socket.\n");
        exit(1);
    }
    fprintf(stderr,"Successfully created socket connection: %d\n",clientSocket);
    memset(&servAddr, '\0', sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Replace this with argv[1] to connect to actual server.

    ret = connect(clientSocket, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if(ret < 0)
    {
        fprintf(stderr,"Connection error: Failed to connect to server.\n");
        exit(2);
    }
    fprintf(stderr,"Connected to server successfully!\n");

    //Now that we're connected... Run this infinitely until exit.
    //Put the free code in here...?
    while(1)
    {
        fprintf(stderr,"Client: \t");
        fgets(buffer, 1024, stdin);
        send(clientSocket, buffer, strlen(buffer), 0);

        if(!(strcmp(buffer, "exit")) || !(strcmp(buffer, "logout")) || !(strcmp(buffer, "bye")))
        {
            close(clientSocket);
            fprintf(stdout,"Disconnected from server.\n");
            break;
        }

        if(recv(clientSocket, buffer, 1024, 0) > 0)
        {
            fprintf(stdout,"Error in receiving response from server.");
        }
        else
        {
            fprintf(stdout,"Server: \t%s\n", buffer);
        }
    }
    /* free the allocated memory */
    free(buffer);
    return(0);
}
