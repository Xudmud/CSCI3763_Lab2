#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include "cat.h"

void clientcat(int sockfd)
{
    unsigned char* buf = calloc(1024,sizeof(unsigned char));
    int len;
    /* Should really recv here. That way if send() tries to send more than 1024 bytes,
     * the buffer can be cleared. */
    while((len = recv(sockfd,buf,1024,0)) > 0)
    {
        fprintf(stdout,"Server says %s",buf);
        if(strcmp(buf,"ITEOTWAWKI") == 0)
            break;
        if(strcmp(buf,"NUMBAHWAN") == 0)
        {
            fprintf(stdout,"Server reported problem with command.\n");
            break;
        }
        fprintf(stdout,"%s",buf);
        bzero(buf,1024);
    }
    free(buf);
    return;
}
