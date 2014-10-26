#include "server.h"

void s_list (char* dir, int socket)
{

    DIR *d;
    struct dirent *dirent;
    struct stat st;
    d = opendir(dir);

    if(d)
    {
        while((dirent = readdir(d))!=NULL)
        {
            if(dirent->d_type != DT_DIR)
            {

                printf("Filename: %s   ", dirent->d_name);

                if(stat(dirent->d_name,&st) == -1){
                    printf("Fehler bei stat.....%s\n", strerror(errno));
                }
                else {
                printf("%ld Bytes\n", st.st_size);
                }
                printf("%ld Bytes\n", st.st_size);
            }



        }
    }

}
