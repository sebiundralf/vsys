#include "server.h"

#define PW_LENGTH 20

#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define FILTER "(uid=if13b067)"
#define BIND_USER "uid=if13b067,ou=People,dc=technikum-wien,dc=at"	/* anonymous bind with user and pw NULL */
#define BIND_PW "asdf12"

/*

return 0 = success
return 1 = error
return 2 = username fail
return 3 = pw fail
return 4 = client close

*/


int server_auth (int socket, int fails)
{

    char fh_uid[9];
    char fh_pw[PW_LENGTH+1];
    char buffer[BUF];
    char name[BUF];

    memset(buffer,'\0',sizeof(buffer));
        memset(name,'\0',sizeof(name));
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
    {
        ldap_unbind(ld);
        return 4;
    }
    strcpy(fh_uid,buffer);
    memset(buffer,'\0',sizeof(buffer));


    /* anonymous bind */
    rc = ldap_simple_bind_s(ld,BIND_USER,BIND_PW);

    if (rc != LDAP_SUCCESS)
    {
        fprintf(stderr,"LDAP error: %s\n",ldap_err2string(rc));
        ldap_unbind(ld);
        return EXIT_FAILURE;
    }


    char searchfilter[17];
    strcpy(searchfilter,"(uid=");
    strcat(searchfilter,fh_uid);
    strcat(searchfilter,")");



    //printf("searchfilter: %s\n",searchfilter);


    rc = ldap_search_s(ld, SEARCHBASE, SCOPE, searchfilter , attribs, 0, &result);



    if (rc != LDAP_SUCCESS)
    {
        strcpy(buffer,"uid error");
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        perror("wrong uid format");

        memset(buffer,'\0',sizeof(buffer));
        ldap_msgfree(result);
        free(attribs[0]);
        free(attribs[1]);
        ldap_unbind(ld);
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
        perror("wrong amount of uids found");

        memset(buffer,'\0',sizeof(buffer));
        ldap_msgfree(result);
        free(attribs[0]);
        free(attribs[1]);
        ldap_unbind(ld);
        return 2;

    }
    /* Name abfragen */
    {
        LDAPMessage *e;
        char *attribute;
        BerElement *ber;		/* array of attributes */
        char **vals;

        e = ldap_first_entry(ld, result);

        for (attribute = ldap_first_attribute(ld,e,&ber); attribute!=NULL; attribute = ldap_next_attribute(ld,e,ber))
        {
            if ((vals=ldap_get_values(ld,e,attribute)) != NULL)
            {
                for (int i=0; vals[i]!=NULL; i++)
                {
                   // printf("i = %d\n",i);
                    if(!strcmp(attribute,"cn"))
                        strcpy(name,vals[i]);
                    //printf("\t%s: %s\n",attribute,vals[i]);
                }
                /* free memory used to store the values of the attribute */
                ldap_value_free(vals);
            }
            /* free memory used to store the attribute */
            ldap_memfree(attribute);
        }
        /* free memory used to store the value structure */
        if (ber != NULL) ber_free(ber,0);
        ldap_memfree(attribute);
        ldap_msgfree(result);
        free(attribs[0]);
        free(attribs[1]);

    }

  //  printf("LDAP search suceeded\n");


    if((w=read(socket,buffer,BUF))==-1)
        perror("Error reading stuff (fh_pw)");

    if(w==0)
    {
        ldap_unbind(ld);
        return 4;


    }
    strcpy(fh_pw,buffer);
    memset(buffer,'\0',sizeof(buffer));

    char user[48];

    strcpy(user, "uid=");
    strcat(user, fh_uid);
    strcat(user, ",ou=People,dc=technikum-wien,dc=at");




    rc = ldap_simple_bind_s(ld,user,fh_pw);


    if (rc ==  LDAP_INVALID_CREDENTIALS)
    {
        sprintf(buffer,"wrong pw%d",++fails);
       // strcpy(buffer,"wrong pw");

        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
        ldap_unbind(ld);
        return 3;
    }


    if (rc != LDAP_SUCCESS)
    {
        fprintf(stderr,"LDAP error: %s\n",ldap_err2string(rc));
        sprintf(buffer,"wrong pw%d",++fails);
        if(write(socket,buffer,BUF)==-1)
            perror("Error writing stuff");

        memset(buffer,'\0',sizeof(buffer));
        ldap_unbind(ld);
        return EXIT_FAILURE;
    }





    strcpy(buffer,"data ok");
    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");

    memset(buffer,'\0',sizeof(buffer));


    do
    {
        if((w=read(socket,buffer,BUF))==-1)
            perror("Error reading stuff (fh_pw)");

        if(w==0)
        {
            ldap_unbind(ld);
            return 4;
        }
    }
    while(strcmp(buffer,"send name"));
    memset(buffer,'\0',sizeof(buffer));

    strcpy(buffer,name);
    if(write(socket,buffer,BUF)==-1)
        perror("Error writing stuff");




    ldap_unbind(ld);
    return 0;
}
