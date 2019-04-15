#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "catalog.h"

void servls(int sockfd)
{
    /* fork the process */
    int child = fork();
    int status;
    if(child < 0)
    {
        perror("fork");
        exit(1);
    }
    else if(child == 0)
    {
        /* child, dup2 */
        dup2(sockfd,1);
        close(sockfd);
        /* Close the socket on the child...? */

        /* call command */
        execlp("ls","ls",(char*)NULL);
        perror("execlp");
        exit(-1);
    }
    else
    {
        /* parent, wait for child */
        waitpid(child, &status, 0);
        if(status != 0)
        {
            send(sockfd,"NUMBAHWAN",9,0);
        }
    }
}