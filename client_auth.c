#include "client.h"

#define PW_LENGTH 20

/*

return 0 = success
return 1 = error
return 2 = username fail
return 3 = pw fail


*/

int client_auth(int socket)
{

    char fh_uid[9];
    char fh_pw[PW_LENGTH+1];

    char buffer[BUF];
    memset(buffer, '\0', sizeof(buffer));

    printf("-------------------------------\n\nLOGIN\n\n");

    printf("Please enter UID (8 Zeichen):\n");

    s_gets(fh_uid,9);

    get_password((char *)&fh_pw,PW_LENGTH);

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

            printf("\nUID NOT VALID: \"%s\"\nPlease try again..\n", fh_uid);
            return 2;
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
        if(!strncmp(buffer,"wrong pw",8))
        {
            int trysleft = 51-buffer[8];

            if(!trysleft){
                printf("You entered 3 times the wrong password, Server blocked ip..\nGood Bye!\n");
                close(socket);
                _exit(0);


            }

            printf("PW NOT CORRECT, %d try(s) left\nPlease try again..\n",trysleft);
            return 3;
        }
    }
    while(strcmp(buffer,"data ok"));
    memset(buffer, '\0', sizeof(buffer));



    printf("Login ok!\n");

    strcpy(buffer,"send name");
    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");

    memset(buffer, '\0', sizeof(buffer));

    if(read(socket,buffer,BUF)==-1)
            perror("Error reading stuff");
clrscr();
    printf("--++ Welcome to vsys_server %s ++--\n\n",buffer);

    strcpy(username,buffer);


    return 0;

}
