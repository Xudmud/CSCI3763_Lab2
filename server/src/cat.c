#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "cat.h"
#include "functions.h"

void servcat(int sockfd, char* comm)
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
        /* Get the file to read */
        getdlfile(&comm);
        /* child, dup2 */
        dup2(sockfd,1);
        close(sockfd);
        /* Close the socket on the child...? */

        /* call command */
        execlp("cat","cat",comm,(char*)NULL);
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
        else
        {
            send(sockfd,"ITEOTWAWKI",10,0);
        }
    }
}
