#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include "download.h"
#include "functions.h"

void servdl(int sockfd, char* comm)
{
    /* Fork the damn thing */
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
        /* Get the filename to open */
        getdlfile(&comm);
        /* Check with client that it can receive file */
        recv(sockfd,buf,1024,0);
        if(strcmp(buf,"1") == 0)
        {
            /* Client failed to open file. Exit. */
            exit(1);
        }
        bzero(buf,1024);
        /* Open the file */
        FILE* ulfile = fopen(comm,"rb");
        if(ulfile == NULL)
        {
            perror("open");
            send(sockfd,"1",1,0);
            free(buf);
            exit(1);
        }
        send(sockfd,"0",1,0);
        recv(sockfd,buf,1024,0);
        bzero(buf,1024);
        /* Next, send the file in chunks until eof */
        fprintf(stdout,"Sending file %s to client...",comm);
        while((len = fread(buf,sizeof(unsigned char),1024,ulfile)) > 0)
        {
            send(sockfd,buf,len,0);
            recv(sockfd,buf,1,0);
            bzero(buf,1024);
        }
        fclose(ulfile);
        bzero(buf,1024);
        /* End of file, send the flag */
        send(sockfd,"ITEOTWAWKI",10,0);
        fprintf(stderr,"File sent!\n");
        exit(0);
    }
    else
    {
        waitpid(child,&status,0);
    }
}


