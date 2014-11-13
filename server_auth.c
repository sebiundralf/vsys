#include "server.h"


#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define FILTER "(uid=if13b067)"
#define BIND_USER NULL 		/* anonymous bind with user and pw NULL */
#define BIND_PW NULL


int server_auth (int socket){

        char* fh_uid;
        char* fh_pw;

        /* GET uid
         *
         *
         *
         *
         *
         *
         */

          LDAP *ld;			/* LDAP resource handle */
   LDAPMessage *result, *e;	/* LDAP result handle */
   BerElement *ber;		/* array of attributes */
   char *attribute;
   char **vals;

   int i,rc=0;

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


    /* anonymous bind */
   rc = ldap_simple_bind_s(ld,BIND_USER,BIND_PW);

   if (rc != LDAP_SUCCESS)
   {
      fprintf(stderr,"LDAP error: %s\n",ldap_err2string(rc));
      return EXIT_FAILURE;
   }

    char searchfilter[14];
    strcpy(searchfilter,"(uid=");
    strcpy(searchfilter,fh_uid);
    strcpy(searchfilter,")");


    rc = ldap_search_s(ld, SEARCHBASE, SCOPE, searchfilter , attribs, 0, &result);






    return 0;
}
