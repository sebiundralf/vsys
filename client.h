#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

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

#define BUF 1024


#define PATH_MAX 255

void c_list (int socket);
void c_get(int socket, char* file);
void c_put(int socket, char* file);


#endif // CLIENT_H_INCLUDED
