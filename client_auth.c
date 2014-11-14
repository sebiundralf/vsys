#include "client.h"

int client_auth(int socket) {

    char fh_uid[9];
    char fh_pw[21];

    char buffer[BUF];
    memset(buffer, '\0', sizeof(buffer));


    printf("Please enter UID (8 Zeichen):\n");

    s_gets(fh_uid,9);

    get_password((char *)&fh_pw,20);

/* Send UID
    strcpy(buffer,fh_uid);

    if(send(socket, msg, strlen (msg), 0)==-1)
        perror("Error sending stuff");

    memset(buffer, '\0', sizeof(buffer));
*/

    /*

        username ok?

    */

    /* send pw


    strcpy(buffer,fh_pw);
   if(send(socket, msg, strlen (msg), 0)==-1)
        perror("Error sending stuff");


        */

    /*

        password ok?


    */






    return 0;

}
