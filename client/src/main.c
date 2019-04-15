#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "catalog.h"
#include "download.h"
#include "ls.h"
#include "pwd.h"
#include "spwd.h"
#include "upload.h"
#include "calendar.h"

#define PORT 24601 //I am Jean Valjean!

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"Usage: wic6774client hostname\n");
        exit(1);
    }
    int clientSocket, ret;
    struct sockaddr_in servAddr;

    struct hostent* he;
    if((he=gethostbyname(argv[1])) == NULL)
    {
        herror("gethostbyname");
        exit(2);
    }
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0)
    {
        perror("socket");
        exit(3);
    }
    fprintf(stdout,"Successfully created socket connection!\n");
    memset(&servAddr, '\0', sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr = *((struct in_addr *)he->h_addr); // Store IP of host specified in argv[1]

    ret = connect(clientSocket, (struct sockaddr*)&servAddr, sizeof(servAddr));
    if(ret < 0)
    {
        perror("Connect");
        exit(4);
    }
    fprintf(stdout,"Connected to server successfully!\n");

    //Now that we're connected... Run this infinitely until exit.
    while(1)
    {
        /* Allocate a command buffer. */
        char* buffer = calloc(1024,sizeof(char*));
        fprintf(stderr,">");
        fgets(buffer, 1024, stdin);
        /* STRIP THE DAMN NEWLINE */
        buffer[strlen(buffer)-1] = '\0';
        /* Send the client message to the server. */
        send(clientSocket, buffer, strlen(buffer), 0);

        /* Process the command */
        /* First, check for an exit statement */
        if(strcmp(buffer, "exit") == 0 || strcmp(buffer, "logout") == 0 || strcmp(buffer, "quit") == 0 || strcmp(buffer, "bye") == 0)
        {
            /* Do I want to receive the goodbye message or just disconnect? */
            close(clientSocket);
            fprintf(stdout,"Disconnected from server.\n");
            free(buffer);
            break;
        }
        /* Next check if the command is pwd or ls, since these are local to the client. */
        else if(strcmp(buffer, "ls") == 0)
        {
            /* Run the ls command. */
            clientls();
        }
        else if (strcmp(buffer, "pwd") == 0)
        {
            /* run the pwd command */
            clientpwd();
        }
        else if(strstr(buffer,"upload") == buffer)
        {
            clientupload(clientSocket, buffer);
        }
        else if(strstr(buffer,"download") == buffer)
        {
            clientdownload(clientSocket, buffer);
        }
        else
        {
            /* Other commands are sent to and need a response from the server */
            if (strcmp(buffer, "spwd") == 0)
            {
                /* Run spwd, server responds with present directory */
                clientspwd(clientSocket);
            }
            else if (strcmp(buffer, "catalog") == 0)
            {
                /* Run catalog, server runs ls and responds with results */
                clientcatalog(clientSocket);
            }
            else if (strcmp(buffer,"calendar") == 0)
            {
                /* Here's the calendar the assignment description said we were doing */
                clientcal(clientSocket);
            }
            else
            {
                fprintf(stdout,"Error: Command not understood.\n");
            }
        }
        /* Clear out extra data */
        int tempfd = open("/dev/null",O_WRONLY);
        if(tempfd < 0)
        {
            perror("fd");
            tempfd = 2;
        }
        send(tempfd, buffer, sizeof(buffer),MSG_DONTWAIT);
        /* Clear out the buffer */
        recv(clientSocket, buffer, sizeof(buffer),MSG_DONTWAIT);
        close(tempfd);
        free(buffer);

    }

    return(0);
}
