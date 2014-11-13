#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <ftw.h>
#include <signal.h>
#include <ldap.h>


#define BUF 1024
#define MAX_PATH 512

void s_list (char* dir, int socket);
void s_get(char* dir,  int socket);
void s_put(char* dir, int socket);
int server_auth (int socket);



#endif // SERVER_H_INCLUDED
