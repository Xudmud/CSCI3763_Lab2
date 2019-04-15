#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ls.h"

void clientls()
{
    /* Fork the process, run ls */
    int child = fork();
    int status;
    if(child < 0)
    {
        perror("fork");
        return;
    }
    else if(child == 0)
    {
        execlp("ls","ls",(char*)NULL);
        perror("exec");
        exit(-1);
    }
    else
    {
        waitpid(child,&status,0);
    }

}