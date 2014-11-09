#include "client.h"



void c_put(int socket, char* file)
{

    if(!file)
    {
        printf("Error! No filename..\n");
        return;
    }

    DIR *dir;
    char path[MAX_PATH];
    char path2[MAX_PATH];
    char buffer[BUF];
    FILE * fp;

    if(getcwd(path, MAX_PATH)==NULL)
        perror("Couldn't get directory");



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
    }

    while(strcmp(buffer,"size ok"));

     memset(buffer, '\0', sizeof(buffer));


    int block_sz;
    int errorf = 1;
    int fname_sent = 0;
    int counter = 0;



    while(!feof(fp))
    /* File übertragen */
    {
        if(!fname_sent) //Name senden
        {
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

            fname_sent= 1;

            strcpy(buffer,"start sending");


            if(write(socket,buffer,BUF)==-1)
                perror("Error writing stuff");
            memset(buffer, '\0', sizeof(buffer));


            do
            {
                if(read(socket,buffer,BUF)==-1)
                    perror("Error reading stuff\n");

                if(!strcmp(buffer,"exit"))
                {
                    perror("Server error\n");
                    return;
                }

            }
            while(strcmp(buffer,"server ok"));

            memset(buffer, '\0', sizeof(buffer));


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
        printf("Couldn't find file %s\n", file);
        memset(buffer,'\0',sizeof(buffer));

        /*  strcpy(buffer, "Sending file failed");
          if(write(socket,buffer,BUF)==-1)
              perror("Error writing stuff");
        */

    }
    else
        printf("File %s was sent to server!\n", file);




    fclose(fp);
    closedir(dir);


}



