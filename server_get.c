#include "server.h"

void s_get(char* dir, char* file, int socket)
{
    DIR *d;
    struct dirent *entry;
    char path[PATH_MAX];
    char buffer[BUF];

    int success = 0;
    char* tmpfile = malloc(sizeof(char*));

    memset(path,0,sizeof(path));

    int temp = (int) strlen(dir);

    if((d = opendir(dir)) != NULL){

        strcat(path,dir);

        if(dir[temp-1] != '/'){
            strcat(path,"/");
        }

        while((entry = readdir(d)) != NULL){

            strcpy(tmpfile,file);
            int flen = strlen(tmpfile);
            tmpfile[flen-1] = '\0';

            if(strcmp(entry->d_name,tmpfile) == 0){
                FILE* fp;
                strcat(path,tmpfile);

                if((fp = fopen(path,"rb")) != NULL){

                    struct stat st;
                    stat(path, &st);

                    char* temp1 = malloc(sizeof(char*));
                    sprintf(temp1,"%ld", (unsigned long)st.st_size);

                    write(socket,temp1,BUF);
                    free(temp1);

                    int block_tr;

                    while(!feof(fp)){
                        memset(buffer,0,sizeof(buffer));
                        if((block_tr = fread(buffer, sizeof(char), BUF, fp)) < 0){
                            perror("ERROR: Could not read file.");
                            return;
                        }
                        if(write(socket, buffer, block_tr) < 0){
                            perror("ERROR: Failed to send file.");
                            return;
                        }
                        success = 1;
                    }
                    fclose(fp);

                }else{
                    perror("ERROR: Could not open file.");
                    return;
                }


            }
        }

    }else{
      perror ("ERROR: Could not open directory.");
      return;
    }

    if(success){
        printf("File %s was transfered successfully.\n", path);
    }else{
        memset(buffer,0,sizeof(buffer));
        strcpy(buffer,"ERROR: Unable to find ");
        strcat(buffer,tmpfile);
        write(socket, buffer, BUF);
    }
        free(tmpfile);
        closedir(d);

}
