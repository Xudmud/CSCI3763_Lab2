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

void clientupload(int sockfd, char* comm)
{
    /* Fork the process */
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
        getulfile(&comm);
        /* Open the file */
        FILE* ulfile = fopen(comm,"rb");
        if(ulfile == NULL)
        {
            perror("open");
            send(sockfd,"1",1,0);
            exit(1);
        }
        send(sockfd,"0",1,0);
        /* Next, make sure the server opened its file. */
        recv(sockfd,buf,1024,0);
        if(strcmp(buf,"1") == 0)
        {
            /* Server had problem. Close the file, free buf, and exit */
            fprintf(stdout,"Server indicated problem opening file. Aborting.\n");
            fclose(ulfile);
            free(buf);
            exit(1);

        }
        bzero(buf,1024);
        /* Next, send the file in chunks until eof */
        fprintf(stdout,"Sending");
        /* While there is still data to be read, read in 1024 bytes */
        while((len = fread(buf,sizeof(unsigned char),1024,ulfile)) > 0)
        {
            /* Send to the server */
            send(sockfd,buf,len,0);
            /* Wait for server to indicate it's ready for the next bit */
            recv(sockfd,buf,1,0);
            /* Zero out the buffer */
            bzero(buf,1024);
        }
        /* Done with the input file, close it */
        fclose(ulfile);
        bzero(buf,1024);
        /* End of file, send the flag */
        send(sockfd,"ITEOTWAWKI",10,0);
        fprintf(stdout,"Done!\n");
        recv(sockfd,buf,3,0);
        if(strcmp("FDE",buf) == 0)
            fprintf(stdout,"Note: File %s was overwritten on server.\n",comm);
        free(buf);
        exit(0);
    }
    else
    {
        /* Parent: Wait for child to finish */
        waitpid(child,&status,0);
    }
}
