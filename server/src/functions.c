#include "functions.h"
#include <stdio.h>

void flushbuf(char* buf)
{
    unsigned char *temp = buf;
    for (unsigned int i = 0; i > sizeof(buf); ++i) {
        *temp = '\0';
        ++temp;
    }
}

void getdlfile(char** comm)
{
    /* Need to advance one space. */
    /* First to advance past the download command */
    while(**comm != '\0' && **comm != ' ')
    {
        (*comm)++;
    }
    (*comm)++;
    char* temp = *comm;
    /* One more, to sanitize any extraneous input */
    while(**comm != '\0' && **comm != ' ')
    {
        (*comm)++;
    }
    **comm = '\0';
    *comm = temp;
}

void getulfile(char** comm)
{
    /* Need to advance two spaces. */
    /* First to advance past the download command */
    while(**comm != '\0' && **comm != ' ')
    {
        (*comm)++;
    }
    (*comm)++;
    /* Second to advance past the server filename */
    while(**comm != '\0' && **comm != ' ')
    {
        (*comm)++;
    }
    (*comm)++;
    char* temp = *comm;
    /* One more, to sanitize any extraneous input */
    while(**comm != '\0' && **comm != ' ')
    {
        (*comm)++;
    }
    **comm = '\0';
    *comm = temp;
}
