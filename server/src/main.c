#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include "catalog.h"
#include "download.h"
#include "upload.h"
#include "spwd.h"

#define PORT 24601 //I am Jean Valjean!

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char const *argv[])
{

    int sockfd, ret;
    struct sigaction sa;
    /* Build a sockaddr object for server and client */
    struct sockaddr_in servAddr;
    struct sockaddr_in cliAddr;
    int newSocket;

    /* Holder for an appropriate size for later sizeof() functions */
    socklen_t addr_size;

    /* Need a memory location set to 1 */
    int sure = 1;

    /* Buffer for commands */
    char* buf = calloc(1024, sizeof(char));

    /* PID for child */
    pid_t child;

    /* Create a socket file descriptor and assign it the current IP */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }
    fprintf(stderr,"Created server socket!\n");
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sure, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(2);
    }
    memset(&servAddr, '\0', sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY; /* Fill with server IP */

    /* Bind the socket */
    ret = bind(sockfd, (struct sockaddr*)&servAddr,sizeof(servAddr));
    if(ret < 0)
    {
        perror("bind");
        exit(3);
    }
    fprintf(stderr,"Successfully bound port!\n");

    /* Listen for connections, with a backlog of 10 */
    if(listen(sockfd,10) == 0)
    {
        fprintf(stderr,"Listening...");
    }
    else
    {
        perror("Listen");
        exit(4);
    }

    /* Collect all dead processes and end them */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(5);
    }

    while(1)
    {
        addr_size = sizeof(cliAddr);

        newSocket = accept(sockfd, (struct sockaddr*)&cliAddr, &addr_size);
        if(newSocket < 0)
        {
            perror("accept");
            continue;
        }
        fprintf(stderr,"Connection accepted from %s:%d\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port));
        int childpid = fork();
        if(childpid < 0)
        {
            fprintf(stderr,"Fork error.\n");
            /* exit...? */
        }
        else if(childpid == 0) /* Child process */
        {
            close(sockfd);
            while(1)
            {
                /* Buffer for commands */
                char* buf = calloc(1024, sizeof(char));
                recv(newSocket, buf, 1024, 0);
                fprintf(stderr,"Client says: %s\n",buf);
                /* Check what the command is and what action needs to be taken. */
                /* First check: ls or pwd, no action is needed by the server. */
                if(strcmp(buf,"ls") == 0 || strcmp(buf,"pwd") == 0)
                {
                    send(newSocket,"Not implemented.\n",17,0);
                }

                /* Next, check for exit statement. */
                else if(strcmp(buf,"exit") == 0 || strcmp(buf,"logout") == 0 || strcmp(buf,"quit") == 0 || strcmp(buf,"bye") == 0)
                {
                    /* Exit command, exit with 0 (child exits) */
                    fprintf(stdout,"Bye. Thank you for using the server!");
                    exit(0);
                }

                    /* Next, determine if the message is supported by the server */
                else
                {
                    /* Need if-statements for each possible command */
                    if(strcmp(buf,"catalog") == 0)
                    {
                        servls();
                        send(newSocket,"Not implemented.\n",17,0);
                    }
                    else if(strcmp(buf,"spwd") == 0)
                    {
                        servpwd();
                        send(newSocket,"Not implemented.\n",17,0);
                    }
                    else if(strcmp(buf,"download") == 0)
                    {
                        servdl();
                        send(newSocket,"Not implemented.\n",17,0);
                    }
                    else if(strcmp(buf,"upload") == 0)
                    {
                        servul();
                        send(newSocket,"Not implemented.\n",17,0);
                    }
                    else
                    {
                        send(newSocket,"Error: Command not recognized.\n", 31, 0);
                    }

                }
                /* Clear out extra data */
                recv(newSocket, buf, sizeof(buf),MSG_DONTWAIT);
                free(buf);
            }
        }
        else
        {
            /* Parent process */
            fprintf(stderr,"Parent waiting for next connection...\n");
        }

    }

    close(sockfd);
    free(buf);
    return(0);
}
