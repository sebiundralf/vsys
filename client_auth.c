#include "client.h"

int client_auth(int socket)
{

    char fh_uid[9];
    char fh_pw[21];

    char buffer[BUF];
    memset(buffer, '\0', sizeof(buffer));

    printf("\nLOGIN\n\n");

    printf("Please enter UID (8 Zeichen):\n");

    s_gets(fh_uid,9);

    get_password((char *)&fh_pw,20);

    /* Send UID */
    strcpy(buffer,fh_uid);

    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");

    memset(buffer, '\0', sizeof(buffer));




    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");
        if(!strcmp(buffer,"uid error"))
        {
            printf("\nUID NOT VALID: %s\n", fh_uid);
        }
    }
    while(strcmp(buffer,"uid ok"));
    memset(buffer, '\0', sizeof(buffer));


    /* send pw */

    strcpy(buffer,fh_pw);
    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");

    memset(buffer, '\0', sizeof(buffer));


    do
    {
        if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");
        if(!strcmp(buffer,"wrong pw"))
        {
            printf("PW NOT CORRECT\n");
            break;
        }
    }
    while(strcmp(buffer,"data ok"));
    memset(buffer, '\0', sizeof(buffer));



    printf("Login ok!\n");




    return 0;

}
