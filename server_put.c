#include "server.h"

void s_put(char* dir, int socket)
{

    char buffer[BUF];
    int fsize;

    strcpy(buffer,"ok");
    write(socket,buffer,BUF);
    memset(buffer,'\0',sizeof(buffer));

    fsize = read(socket, buffer, BUF);

    if(strncmp(buffer,"Sending file failed",18) == 0)
        printf("%s \n",buffer);
    else
    {
        long filesize = atol(buffer);
        memset(buffer,'\0',sizeof(buffer));


        FILE *fp;

        char path[512];
        char file_name[512];
        memset(path,'\0',sizeof(path));


        strcpy(buffer,"ok2");
        write(socket,buffer,BUF);
        memset(buffer,'\0',sizeof(buffer));

        read(socket,buffer,BUF);

        strcpy(file_name,buffer);

        memset(buffer,'\0',sizeof(buffer));

        strcpy(buffer,"ok3");
        write(socket,buffer,BUF);
        memset(buffer,'\0',sizeof(buffer));


        strcpy(path,dir);
        int t = (int) strlen(path);
        if(path[t-1]!='/')
            strcat(path,"/");

        strcat(path,file_name);
       // printf("Filename + path: %s\n", path);
        if((fp = fopen(path,"w"))==NULL)
        {

            perror("Could not open file");
            return;
        }

        do
        {
            memset(buffer,'\0',sizeof(buffer));
            fsize = recv(socket,buffer,BUF,0);
            fwrite(buffer, sizeof(char),BUF,fp);
            filesize -= (long) fsize;




        }
        while (filesize>0);
        fclose(fp);

        printf("file put successful\n");

    }



}
