#include "server.h"

void s_get(char* dir, int socket)
{
    char buffer[BUF];
    char path[PATH_MAX];
    memset(buffer,'\0',sizeof(buffer));
    strcpy(path, dir);

    /* Verbindungsaufbau */

    {
        strcpy(buffer,"server ready");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
    }






    char file_name[MAX_PATH];
    FILE * fp;




    if(read(socket,buffer,BUF)==-1)
        perror("Error reading stuff (filename)");

    strcpy(file_name,buffer);

    memset(buffer,'\0',sizeof(buffer));

    strcpy(buffer,"filename ok");
    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");


    memset(buffer,'\0',sizeof(buffer));

    printf("Filename \"%s\" recieved\n", file_name);




    int l = strlen(path);

    if(path[l-1] != '/')
    {
        strcat(path,"/");
    }
    strcat(path,file_name);


    printf("%s\n", path);


    if((fp = fopen(path, "rb")) == NULL)
    {
        printf("Couldn't open file \"%s\"\n",file_name);
        strcpy(buffer,"exit");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
        return;
    }


    strcpy(buffer,"start");
    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");

    memset(buffer,'\0',sizeof(buffer));

    /* PUT BEGINNT */



    int fsize;

    fseek(fp,0,SEEK_END);
    fsize=ftell(fp);
    fseek(fp,0,SEEK_SET);



    if(write(socket,(void*)&fsize,sizeof(int))==-1)
        perror("Error writing stuff");

    printf("File size: %i b\n", fsize);



    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");
        if(!strcmp(buffer,"exit"))
        {
            printf("Error client, get failded");
            fclose(fp);

            return;
        }
    }

    while(strcmp(buffer,"size ok"));

    memset(buffer, '\0', sizeof(buffer));


    int block_sz;
    int errorf = 1;

    int counter = 0;

    printf("start sending..\n");

    while(!feof(fp))
        /* File übertragen */
    {


        memset(buffer, '\0', sizeof(buffer));


        if((block_sz = fread(buffer,1, sizeof(buffer)-1, fp))<0)
        {
            printf("Error: couldn't read file\n");
            return;

        }


        int status;
        do
        {
            status = write(socket, buffer, block_sz);
        }

        while(status<0);
        printf("Block %d sent\n", counter);
        counter++;
        errorf=0;
        //break;
    }


    if(errorf)
    {
        printf("Couldn't find file %s\n", file_name);
        memset(buffer,'\0',sizeof(buffer));

        /*  strcpy(buffer, "Sending file failed");
          if(write(socket,buffer,BUF)==-1)
              perror("Error writing stuff");
        */

    }
    else
        printf("File %s was sent to client!\n", file_name);


    fclose(fp);
}
