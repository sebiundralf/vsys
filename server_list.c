#include "server.h"

//int list(const char*, const struct stat*, int type);

void s_list (char* dir, int socket)
{

    DIR *d;
    struct dirent *entry;
    char path[PATH_MAX];
    char buffer[BUF];


    memset(buffer,'\0',sizeof(buffer));
    {

        strcpy(buffer,"server ready");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
    }
    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");
    }

    while(strcmp(buffer,"start"));

    printf("start listing..\n");

    //ftw(dir, list, 1);


    memset(buffer, 0, sizeof(buffer));

    // printf("Dir: %s\n", dir);
    if ((d = opendir(dir)) != NULL)
    {

        while((entry = readdir(d)) !=NULL)
        {
            memset(path,0,sizeof(path));


            if(entry->d_type != DT_DIR)
            {

                int bufsize= (int)strlen(buffer);
                int filesize= (int)strlen(entry->d_name);
                struct stat st;

                strcat(path, dir);
                int temp= (int) strlen(dir);

                if(dir[temp-1]!='/')
                    strcat(path, "/");

                strcat(path,entry->d_name);
                stat(path, &st);

                char *tmp = malloc(sizeof(char*)*12);
                //printf("Entry: %s size: %ld\n", entry->d_name,(unsigned long)st.st_size);
                sprintf(tmp, "%ld", (unsigned long)st.st_size);

                int tmpsize = (int)strlen(tmp);
                tmp[tmpsize] = '\0';

                if(BUF - bufsize >= (tmpsize+filesize+20))
                {
                    strcat(buffer,entry->d_name);
                    strcat(buffer," || Filesize: ");
                    strcat(buffer,tmp);
                    strcat(buffer, " byte\n");
                    free(tmp);
                }
                else
                {
                    if(write(socket, buffer, BUF)==-1)
                        perror("Error writing stuff");
                    memset(buffer,0,sizeof(buffer));
                    strcpy(buffer,entry->d_name);
                    strcat(buffer," || Filesize: ");
                    strcat(buffer,tmp);
                    strcat(buffer, "B\n");
                    free(tmp);


                }
            }

        }


    }
    else
    {
        perror ("Could not open directory");
        return;
    }

    if(strlen(buffer) != 0)
    {
        if(write(socket, buffer, BUF)==-1)
            perror("Error writing stuff");
        memset(buffer,0,sizeof(buffer));
    }

    if(write(socket, buffer, BUF)==-1)
        perror("Error writing stuff");
    closedir(d);


    printf("Sending list successful\n");


}




/*
int list(const char *name,const struct stat *status, int type)
{

 stat(name, status);
 int size= status->st_size;

 if(type == FTW_NS)
  return 0;

 if(type == FTW_F)
  printf("%s || %d Bytes\n", name, size);


 return 0;
}
*/
