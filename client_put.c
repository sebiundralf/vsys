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
        printf("Couldn't open file \"%s\"\n",path);
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
    /*
    while(!feof(fp)){

        mem
    */

    fclose(fp);
    closedir(path2);
}



