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
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BLOCKTIME 30
#define BUF 1024
#define MAX_PATH 512


struct blocked_adress
{
    struct in_addr sin_addr;
    time_t t;
    struct tm* timestamp;
    struct blocked_adress *next;
};

typedef struct blocked_adress blacklist;


void s_list (char* dir, int socket);
void s_get(char* dir,  int socket);
void s_put(char* dir, int socket);
int server_auth (int socket, int fails);
void clear_list(blacklist *head);
void add_blacklist_entry(blacklist**head, struct in_addr ip);
int check_blacklist(blacklist**head, struct in_addr ip);





#endif // SERVER_H_INCLUDED
