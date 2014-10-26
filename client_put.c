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

    printf("%s\n", path);


    if((fp = fopen(path, "rb")) == NULL)
    {
        printf("Couldn't open file \"%s\"\n",file);
        return;

    }

    struct stat st;
    stat(path, &st);

    char* temp = malloc(sizeof(char*));
    memset(temp,'\0',sizeof(temp));
    sprintf(temp,"%ld", (unsigned long) st.st_size);

    // printf("%s", temp);

    write(socket,temp,BUF);
    free(temp);

    int block_sz;

    while(!feof(fp)){

        memset(buffer, '\0', sizeof(buffer));
        if((bock_sz = fread(buffer,sizeof(char), STR_SIZE, fp))<0) {
            printf("Error: couldn't read file\n");
            return;

        }
        if(write(socket, buffer, block_sz)<0){
            fprintf(stderr, "Error: Coudn't send file %s.\n", file);
            return;
        }
    }

    printf("File %s was sent to server!\n", file);




    fclose(fp);
    closedir(path2);
}



