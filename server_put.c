#include "server.h"

void s_put(char* dir, int socket)
{

    char buffer[BUF];
    int fsize;

    memset(buffer,'\0',sizeof(buffer));

    fsize = read(socket, buffer, BUF);

    long s = strtol(buffer,(char**)NULL,30);

    printf("buffer %s", buffer);
    printf("size %d", fsize);
    printf("s %ld\n", s);



}
