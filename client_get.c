#include "client.h"


void c_get(int socket, char* file)
{

    DIR *dir;
    char path[MAX_PATH];
    char path2[MAX_PATH];
    char buffer[BUF];
    FILE * fp;


    if(!file)
    {
        printf("Error! No filename..\n");
        return;
    }


    char* msg = "get";


    if(send(socket, msg, strlen (msg), 0)==-1)
        perror("Error sending stuff");

    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");
    }

    while(strcmp(buffer,"server ready"));

    printf("%s\n",buffer);


    memset(buffer, '\0', sizeof(buffer));
    strcpy(buffer,file);


    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");
    memset(buffer, '\0', sizeof(buffer));
    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");

    }
    while(strcmp(buffer,"filename ok"));

    printf("Filename sent\n");




}
