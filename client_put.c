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

    struct stat st;
    stat(path, &st);

    char* temp = malloc(sizeof(char*));
    memset(temp,'\0',sizeof(temp));
    sprintf(temp,"%ld", (unsigned long) st.st_size);

    // printf("%s", temp);
    //  char * test = "tetst\n";
    write(socket,temp,BUF);
    free(temp);


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
        if((block_sz = fread(buffer,sizeof(char), BUF, fp))<0)
        {
            printf("Error: couldn't read file\n");
            return;

        }
        if(send(socket, buffer, block_sz,0)<0)
        {
            fprintf(stderr, "Error: Coudn't send file %s.\n", file);
            return;
        }
        errorf=0;
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



