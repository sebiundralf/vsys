#include "client.h"




void c_get(int socket, char* file)
{
    printf("Get wird ausgeführt\n");
    char buffer[BUF];

    char* msg = "get";

    if(send(socket, msg, strlen (msg), 0)==-1)
        perror("Error sending stuff");

    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");

        if(!strcmp(buffer,"log"))
        {
            printf("Error in GET, you must login first, send command: \"LOGIN\"\n\n");
            return;
        }
    }

    while(strcmp(buffer,"server ready"));

    printf("%s\n",buffer);
    FILE * fp;

    if(!file)
    {
        printf("Error! No filename..\n");
        return;
    }


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

    memset(buffer, '\0', sizeof(buffer));
    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");
        if(!strcmp(buffer,"exit"))
        {
            printf("Server error, couldn't open file %s\n", file);
            return;


        }

    }
    while(strcmp(buffer,"start"));
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

        int filesize = fsize;
        // int fsizemsg = fsize;
        fsize = 0;
        memset(buffer,'\0',sizeof(buffer));

        char path[MAX_PATH];

        memset(path,'\0',sizeof(path));

        if(getcwd(path, MAX_PATH)==NULL)
            perror("Couldn't get directory");


        int t = (int) strlen(path);
        if(path[t-1]!='/')
            strcat(path,"/");

        strcat(path,file);
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

        strcpy(buffer,"size ok");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));

        {
            struct timeval timeout = {10,0};
            fd_set fds;

            int buffer_fd;

            memset(buffer,'\0',sizeof(buffer));

            printf("sending started...");


            int prozent = filesize /100;
            long filesize2 = filesize;

            int progress = 0;
            int i = 0;
            if(prozent)
                i = progress/prozent;
            int i2 = 0;

            printf("Downloading %s....\nDownload progress: %2d %%\n",file,i);
            i2 = i;

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

                    ws = fwrite(buffer, 1,rs,fp);
                    memset(buffer,'\0',sizeof(buffer));

                    //printf("rs: %d\n", rs);


                    progress += rs;

                    if(progress==filesize2)
                        i = 100;
                    else
                        i = progress/prozent;

                    if(i>i2)
                    {
                        clrscr();
                        printf("Downloading %s....\nDownload progress: %2d %%\n",file,i);
                    }

                    i2 = i;


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
            printf("Got File successfully\n");

        }

    }





}
