#include "server.h"

//int list(const char*, const struct stat*, int type);

void s_list (char* dir, int socket)
{
    //ftw(dir, list, 1);
    DIR *d;
    struct dirent *entry;
    char path[255];
    char buffer[BUF];

    memset(buffer, 0, sizeof(buffer));

    if ((d = opendir(dir)) != NULL){

        while((entry = readdir(d)) !=NULL){
            memset(path,0,sizeof(path));

            if(entry->d_type != DT_DIR){

                int bufsize= (int)strlen(buffer);
                int filesize= (int)strlen(entry->d_name);
                struct stat st;

                strcat(path, dir);
                int temp= (int) strlen(dir);

                if(dir[temp-1]!='/')
                    strcat(path, "/");

                strcat(path,entry->d_name);
                stat(path, &st);

                char *tmp = malloc(sizeof(char*));
                sprintf(tmp, "%ld", (unsigned long)st.st_size);

                int tmpsize = (int)strlen(tmp);
                tmp[tmpsize] = '\0';

                if(BUF - bufsize >= (tmpsize+filesize+20)){
                    strcat(buffer,entry->d_name);
                    strcat(buffer," || Filesize: ");
                    strcat(buffer,tmp);
                    strcat(buffer, "B\n");
                    free(tmp);
                }else{
                    write(socket, buffer, BUF);
                    memset(buffer,0,sizeof(buffer));
                    strcpy(buffer,entry->d_name);
                    strcat(buffer," || Filesize: ");
                    strcat(buffer,tmp);
                    strcat(buffer, "B\n");
                    free(tmp);


                }
            }

        }


    }else{
      perror ("Could not open directory");
      return EXIT_FAILURE;
    }

    if(strlen(buffer) != 0){
        write(socket, buffer, BUF);
        memset(buffer,0,sizeof(buffer));
    }
    write(socket, buffer, BUF);
    closedir(d);

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
