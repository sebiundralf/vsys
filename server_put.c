#include "server.h"

void s_put(char* dir, int socket)
{
    /* Verbindungsaufbau */
    char buffer[BUF];
    {




        strcpy(buffer,"server ready");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
    }

    /* Put beginnt */

    {
        int fsize, statt;
        int rs; //reading size
        int ws; //writing size
        do
        {
            statt = read(socket, &fsize, sizeof(int));
        }
        while(statt<0);

        printf("Filesize recived: %d bytes\n", fsize);

        strcpy(buffer,"size ok");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));



        //if(strncmp(buffer,"Sending file failed",18) == 0)
        //  printf("%s \n",buffer);

        //else
        {
            int filesize = fsize;
            // int fsizemsg = fsize;
            fsize = 0;
            memset(buffer,'\0',sizeof(buffer));


            FILE *fp;

            char path[MAX_PATH];
            char file_name[MAX_PATH];
            memset(path,'\0',sizeof(path));



            if(read(socket,buffer,BUF)==-1)
                perror("Error reading stuff (filename)");

            strcpy(file_name,buffer);

            memset(buffer,'\0',sizeof(buffer));

            strcpy(buffer,"filename ok");
            if(write(socket,buffer,BUF)==-1)
                perror("Error writing stuff");


            memset(buffer,'\0',sizeof(buffer));

            printf("Filename \"%s\" recieved\n", file_name);



            strcpy(path,dir);
            int t = (int) strlen(path);
            if(path[t-1]!='/')
                strcat(path,"/");

            strcat(path,file_name);
            printf("Filename + path: %s\n", path);



            if((fp = fopen(path,"w"))==NULL)
            {

                perror("Could not open file");

                strcpy(buffer,"exit");
                if(write(socket,buffer,BUF)==-1)
                    perror("Error writing stuff");

                memset(buffer,'\0',sizeof(buffer));
                return;
            }




            struct timeval timeout = {10,0};
            fd_set fds;

            int buffer_fd;


            do
            {
                if(read(socket,buffer,BUF)==-1)
                    perror("Error reading stuff");

            }
            while(strcmp(buffer,"start sending"));
            memset(buffer,'\0',sizeof(buffer));

            strcpy(buffer,"server ok");
            if(write(socket,buffer,BUF)==-1)
                perror("Error writing stuff");

            memset(buffer,'\0',sizeof(buffer));


            while (filesize>0)
            {

                FD_ZERO(&fds);
                FD_SET(socket,&fds);

                buffer_fd= select(FD_SETSIZE,&fds,NULL,NULL,&timeout);
                //buffer_fd = 1;

                if(buffer_fd < 0)
                {
                    perror("bad file descriptor set.\n");
                }
                if(buffer_fd == 0)
                {
                    perror("buffer read timeout expired\n");
                }


                if(buffer_fd >0)
                {
                    do
                    {
                        rs = read(socket,buffer,BUF);
                    }
                    while(rs<0);


                    //    memset(buffer,'\0',sizeof(buffer));
                    // fsize = recv(socket,buffer,BUF,0);

                    ws = fwrite(buffer, 1,rs,fp);
                    memset(buffer,'\0',sizeof(buffer));

                    printf("rs: %d\n", rs);


                    if(rs != ws)
                    {

                        perror("error in read write\n");
                        printf("rs %d, ws %d\n",rs,ws);
                    }


                    filesize -= rs;
                    // break;
                }



            }

            fclose(fp);

            //  printf("Submit failed!\n");
            printf("file put successful\nFilename: %s\nFilesize: %d B\n",file_name,rs);

        }

    }

}
