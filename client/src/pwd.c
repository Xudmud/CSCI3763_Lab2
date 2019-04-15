#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pwd.h"

void clientpwd()
{
    /* Fork the process, run pwd */
    int child = fork();
    int status;
    if(child < 0)
    {
        perror("fork");
        return;
    }
    else if(child == 0)
    {
        execlp("pwd","pwd",(char*)NULL);
        perror("exec");
        exit(-1);
    }
    else
    {
        waitpid(child,&status,0);

    }

}