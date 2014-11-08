#include "server.h"

void s_put(char* dir, int socket)
{

    char buffer[BUF];
    int fsize, statt;

    strcpy(buffer,"ok");
  if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");
    memset(buffer,'\0',sizeof(buffer));

    do
    {
        statt = read(socket, &fsize, sizeof(int));
    }

    while(statt<0);


    if(strncmp(buffer,"Sending file failed",18) == 0)
        printf("%s \n",buffer);
    else
    {
       // int filesize = fsize;
       // int fsizemsg = fsize;
        fsize = 0;
        memset(buffer,'\0',sizeof(buffer));


        FILE *fp;

        char path[512];
        char file_name[512];
        memset(path,'\0',sizeof(path));


        strcpy(buffer,"ok2");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");
        memset(buffer,'\0',sizeof(buffer));

        if(read(socket,buffer,BUF))
            perror("Error reading stuff");

        strcpy(file_name,buffer);

        memset(buffer,'\0',sizeof(buffer));

        strcpy(buffer,"ok3");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");
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




        /*
                struct timeval timeout = {10,0};
            fd_set fds;

             int buffer_fd;
                while (filesize>0)
                {

                    FD_ZERO(&fds);
                    FD_SET(socket,&fds);

                   buffer_fd= select(FD_SETSIZE,&fds,NULL,NULL,&timeout);
                    buffer_fd = 1;

                    if(buffer_fd < 0)
                    {
                        perror("bad file descriptor set.\n");
                    }
                    if(buffer_fd == 0)
                    {
                        perror("buffer read timeout expired\n");
                    }

                    int rs;
                    int ws;
                    if(buffer_fd >0)
                    {
                        do
                        {
                            rs = read(socket,buffer,BUF);


                        }
                        while(rs<0);


                        memset(buffer,'\0',sizeof(buffer));
                        fsize = recv(socket,buffer,BUF,0);

                        ws = fwrite(buffer, 1,rs,fp);
                        memset(buffer,'\0',sizeof(buffer));


                        if(fsize != ws)
                        {

                            perror("error in read write\n");
                            printf("fsize %d, ws %d\n",fsize,ws);
                        }


                        filesize -= fsize;
                        break;
                    }



                }
        */
        fclose(fp);

        printf("Submit failed!\n");
        //  printf("file put successful\nFilename: %s\nFilesize: %d B\n",file_name,fsize,fsizemsg);

    }



}
