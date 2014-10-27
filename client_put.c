#include "client.h"


void c_put(int socket, char* file)
{

    if(!file)
    {
        printf("Error! No filename..\n");
        return;
    }

    DIR *dir;
    char path[512];
    char path2[512];
    char buffer[BUF];
    FILE * fp;

    getcwd(path, 512);



    if((dir=opendir(path)) == NULL)
    {
        printf("Couldn't open currend directory.\n");
        return;
    }
    strcpy(path2, path);

    int l = strlen(path);

    if(path[l-1] != '/')
    {
        strcat(path,"/");
    }
    strcat(path,file);

    //  printf("%s\n", path);


    if((fp = fopen(path, "rb")) == NULL)
    {
        printf("Couldn't open file \"%s\"\n",file);
        return;

    }

    char* msg = "put";


    send(socket, msg, strlen (msg), 0);

    do
    {
        read(socket,buffer,BUF);
    }

    while(strcmp(buffer,"ok"));


    int fsize;

    fseek(fp,0,SEEK_END);
    fsize=ftell(fp);
    fseek(fp,0,SEEK_SET);

    printf("File size: %i b\n", fsize);

    write(socket,(void*)&fsize,sizeof(int));


    int block_sz;
    int errorf = 1;
    int fname_sent = 0;

    while(!feof(fp))
    {
        if(!fname_sent)
        {
            memset(buffer, '\0', sizeof(buffer));
            do
            {
                read(socket,buffer,BUF);

            }
            while(strcmp(buffer,"ok2"));
            memset(buffer, '\0', sizeof(buffer));
            strcpy(buffer,file);
            write(socket,buffer,BUF);
            memset(buffer, '\0', sizeof(buffer));
            do
            {
                read(socket,buffer,BUF);

            }
            while(strcmp(buffer,"ok3"));


        }

        memset(buffer, '\0', sizeof(buffer));


        if((block_sz = fread(buffer,1, sizeof(buffer)-1, fp))<0)
        {
            printf("Error: couldn't read file\n");
            return;

        }


        int status;
        do
        {
            //status = write(socket, buffer, block_sz);
        }
        while(status<0);

        errorf=0;
        break;
    }


    if(errorf)
    {
        printf("Couldn't find file %s\n", file);
        memset(buffer,'\0',sizeof(buffer));
        strcpy(buffer, "Sending file failed");
        write(socket,buffer,BUF);


    }
    else
        printf("File %s was sent to server!\n", file);




    fclose(fp);
    closedir(dir);
}



