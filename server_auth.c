#include "server.h"


#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define FILTER "(uid=if13b067)"
#define BIND_USER NULL	/* anonymous bind with user and pw NULL */
#define BIND_PW NULL

/*

return 0 = success
return 1 = error
return 2 = username fail
return 3 = pw fail
return 4 = client close

*/


int server_auth (int socket)
{

    char fh_uid[9];
    char fh_pw[21];
    char buffer[BUF];
    memset(buffer,'\0',sizeof(buffer));
    int w;

    LDAP *ld;			/* LDAP resource handle */
    LDAPMessage *result;	/* LDAP result handle */


    int rc=0;

    char *attribs[3];		/* attribute array for search */

    attribs[0]=strdup("uid");		/* return uid and cn of entries */
    attribs[1]=strdup("cn");
    attribs[2]=NULL;		/* array must be NULL terminated */


    /* setup LDAP connection */
    if ((ld=ldap_init(LDAP_HOST, LDAP_PORT)) == NULL)
    {
        perror("ldap_init failed");
        return EXIT_FAILURE;
    }
    /* get UID  */
    if((w=read(socket,buffer,BUF))==-1)
        perror("Error reading stuff (fh_uid)");

    if(w==0)
        return 4;

    strcpy(fh_uid,buffer);
    memset(buffer,'\0',sizeof(buffer));


    /* anonymous bind */
    rc = ldap_simple_bind_s(ld,BIND_USER,BIND_PW);

    if (rc != LDAP_SUCCESS)
    {
        fprintf(stderr,"LDAP error: %s\n",ldap_err2string(rc));
        return EXIT_FAILURE;
    }


    char searchfilter[17];
    strcpy(searchfilter,"(uid=");
    strcat(searchfilter,fh_uid);
    strcat(searchfilter,")");



    printf("searchfilter: %s\n",searchfilter);


    rc = ldap_search_s(ld, SEARCHBASE, SCOPE, searchfilter , attribs, 0, &result);



    if (rc != LDAP_SUCCESS)
    {
        strcpy(buffer,"uid error");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
            ldap_msgfree(result);
    free(attribs[0]);
    free(attribs[1]);
        return 2;
    }


    if(ldap_count_entries(ld, result)==1)
    {
        strcpy(buffer,"uid ok");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
    }
    else
    {
        strcpy(buffer,"uid error");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");


        memset(buffer,'\0',sizeof(buffer));
            ldap_msgfree(result);
    free(attribs[0]);
    free(attribs[1]);
        return 2;

    }

    ldap_msgfree(result);
    free(attribs[0]);
    free(attribs[1]);
    printf("LDAP search suceeded\n");


    if((w=read(socket,buffer,BUF))==-1)
        perror("Error reading stuff (fh_pw)");

      if(w==0)
        return 4;

    strcpy(fh_pw,buffer);
    memset(buffer,'\0',sizeof(buffer));


    attribs[0]=strdup("uid");		/* return uid and cn of entries */
    attribs[1]=strdup("cn");
    attribs[2]=NULL;		/* array must be NULL terminated */

    char user[48];

    strcpy(user, "uid=");
    strcat(user, fh_uid);
    strcat(user, ",ou=People,dc=technikum-wien,dc=at");




    rc = ldap_simple_bind_s(ld,user,fh_pw);


    if (rc ==  LDAP_INVALID_CREDENTIALS)
    {
        strcpy(buffer,"wrong pw");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
            ldap_msgfree(result);
    free(attribs[0]);
    free(attribs[1]);
    return 3;
    }


    if (rc != LDAP_SUCCESS)
    {
        fprintf(stderr,"LDAP error: %s\n",ldap_err2string(rc));
        return EXIT_FAILURE;
    }





    strcpy(buffer,"data ok");
    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");

    memset(buffer,'\0',sizeof(buffer));




    return 0;
}
