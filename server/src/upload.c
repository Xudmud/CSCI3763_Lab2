#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "download.h"
#include "functions.h"

void servul(int sockfd, char* comm)
{
    /* Fork a child, to get rid of this fucking extra data */
    int child = fork();
    int status;
    if(child < 0)
    {
        perror("fork");
        return;
    }
    else if(child == 0)
    {
        /* use an unsigned char, since files are being transferred in binary mode. */
        unsigned char* buf = calloc(1024,sizeof(unsigned char));
        int len;
        int exists = 0;
        /* Get the filename to open. */
        getulfile(&comm);
        if(access(comm,F_OK) == 0)
        {
            exists = 1;
        }
        /* Receive signal from client that file is open */
        recv(sockfd,buf,1024,0);
        if(strcmp(buf,"1") == 0)
        {
            /* Client indicated problem. Free buf and exit. */
            free(buf);
            exit(1);
        }
        bzero(buf,1024);
        /* Open the file */
        FILE* dlfile = fopen(comm,"wb");
        if(dlfile == NULL)
        {
            /* Indicate to client there was a problem and exit */
            perror("open");
            send(sockfd,"1",1,0);
            exit(1);
        }
        send(sockfd,"0",1,0);
        fprintf(stdout,"Receiving...");
        /* Next, continue to read from the server until the "end" message comes in */
        while((len = recv(sockfd,buf,1024,0)) > 0)
        {
            if(strcmp(buf,"ITEOTWAWKI") == 0)
                break;
            //fprintf(stderr,"Message received: %s\n",buf);
            fwrite(buf,sizeof(unsigned char),len,dlfile);
            bzero(buf,1024);
            send(sockfd,"0",1,0);
        }
        fclose(dlfile);
        if(exists)
            send(sockfd,"FDE",3,0);
        else
            send(sockfd,"FNE",3,0);
        /* Free the socket */
        recv(sockfd,buf,1024,0);
        free(buf);
        exit(0);
    }
    else
    {
        waitpid(child,&status,0);
    }
}
