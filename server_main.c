#include "server.h"

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

char* vdir = NULL; //Verzeichnispfad, muss später freigegeben werden
int create_socket;
void checkdir(char* dir);


blacklist *head = NULL;

int fd[2];

/* Funktion print_usage() zur Ausgabe der usage Meldung */
void print_usage()
{
    fprintf(stderr,"\nUsage: vsys_server -p PORT -d DIRECTORY (Absolute or subfolders) \n\n");
    exit(EXIT_FAILURE);
}

/* Signal handler für STRG + C (zum beenden) */

void strgc_handler(int sig)
{

    if(vdir)
    {
        free(vdir);
    }
    if(create_socket)
        close(create_socket);

    clear_list(head);
    /* Clear blacklist() */

    printf("\n\n...Server wurde beendet.\n");
    _exit(0);
}

void sig_handler(int sig)
{
    //close (fd[1]);

    char buffer2[BUF];
    if(read(fd[0],buffer2,BUF)==-1)
        perror("Error reading stuff");


    struct sockaddr_in ad;
    memset(&ad,0,sizeof(ad));
    inet_aton(buffer2,&ad.sin_addr);
    // printf("IP: %s",buffer2);
    add_blacklist_entry(&head, ad.sin_addr);



}

void hup_handler(int sig){

 pid_t kind1 = wait(NULL);
 //printf("Kind: %d wurde beendet\n",kind1);

}

int main(int argc, char* argv[])

{
    long pid = getpid();
    int vport = -1; //Portnummer
    (void) signal(SIGINT,strgc_handler);
    (void) signal(SIGTERM,sig_handler);
     (void) signal(SIGHUP,hup_handler);

    if (pipe (fd) < 0)
    {
        perror ("pipe");
        exit (EXIT_FAILURE);
    }

    /* Start der GETOPT behandlung */
    {
        int c;
        int counter = 0;
        int sl = 0;

        if(argc <3)
            print_usage();

        while( (c = getopt(argc, argv, "d:p:")) != EOF )
        {

            switch (c)
            {
            case 'p':
                if(vport!=-1)
                {
                    print_usage();
                    break;
                }
                vport = strtol(optarg,(char**)NULL,10);
                counter++;
                break;

            case 'd':
                if(vdir!=NULL)
                {
                    print_usage();
                    break;
                }
                sl = strlen(optarg);
                vdir = (char*) malloc(sizeof(char)* (sl+1));
                strcpy(vdir, optarg);
                counter++;
                break;

            case '?':
                print_usage();
                break;
            }
        }


        //  printf("\nPfad: %s, Port: %d\n",vdir, vport);

        //    printf("\nCounter: %d; optind: %d; argc: %d\n", counter, optind, argc);

        if(counter!= 2 || optind!=argc || vport < 1)
            print_usage();
        //printf("perfekt");


        /* ENDE GETOPT*/
    }


    /* Überprüfen ob korrektes Verzeichnis angegeben wurde */

    checkdir(vdir);



    /* ANFANG Verbindungsaufbau */
    {
        int new_socket, new_socket0;
        socklen_t addrlen;
        char buffer[BUF];
        int size;
        struct sockaddr_in address, cliaddress;
        pid_t process;

        create_socket = socket (AF_INET, SOCK_STREAM, 0);

        memset(&address,0,sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons (vport);

        if (bind ( create_socket, (struct sockaddr *) &address, sizeof (address)) != 0)
        {
            perror("bind error");
            return EXIT_FAILURE;
        }
        listen (create_socket, 5);

        addrlen = sizeof (struct sockaddr_in);
        printf("Waiting for connections...\n");

        while (1)
        {

            new_socket0 = accept ( create_socket, (struct sockaddr *) &cliaddress, &addrlen );

            printf("client try's to connect..\n");

            long blockleft = check_blacklist(&head,cliaddress.sin_addr);
            if(blockleft)
            {

                printf("Client blocked form: %s\n",inet_ntoa (cliaddress.sin_addr));
                sprintf(buffer,"block|Connection refused!\nYour are blocked from the server.\nTime remaining: %ld minutes and %ld seconds|",blockleft/60,blockleft%60);
                if(send(new_socket0, buffer, strlen (buffer), 0)==-1)
                    perror("Error sending stuff");
                close (new_socket0);


                continue;
            }

            process = fork();
            if (process == -1)
            {
                new_socket=new_socket0;
                if (new_socket > 0)
                {
                    strcpy(buffer,"fail");
                    if(send(new_socket0, buffer, strlen (buffer), 0)==-1)
                        perror("Error sending stuff");
                }

                return 1;
            }

            if(process==0)
            {
                int fails = 0;
                int auth = 0;
                new_socket = new_socket0;
                if (new_socket > 0)
                {
                    printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr),ntohs(cliaddress.sin_port));
                    strcpy(buffer,"win");
                    if(send(new_socket0, buffer, strlen (buffer), 0)==-1)
                        perror("Error sending stuff");
                }

                do
                {
                    size = recv (new_socket, buffer, BUF-1, 0);
                    if( size > 0)
                    {
                        buffer[size] = '\0';
                        // printf ("Message received: %s\n", buffer);
                        /* Befehl auslesen */
                        {
                            if(!strcasecmp(buffer, "LIST"))
                            {
                                if(auth)
                                {
                                    strcpy(buffer,"server ready");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error writing stuff");

                                    memset(buffer,'\0',sizeof(buffer));

                                    printf("List wird ausgeführt\n");
                                    s_list(vdir, new_socket);
                                }
                                else
                                {
                                    strcpy(buffer,"log");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error writing stuff");
                                    memset(buffer,'\0',sizeof(buffer));

                                }

                            }
                            else if(!strcasecmp(buffer, "GET"))
                            {
                                if(auth)
                                {
                                    strcpy(buffer,"server ready");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error writing stuff");
                                    printf("Get wird ausgeführt\n");
                                    s_get(vdir, new_socket);
                                }
                                else
                                {
                                    strcpy(buffer,"log");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error writing stuff");
                                    memset(buffer,'\0',sizeof(buffer));

                                }



                            }
                            else if(!strcasecmp(buffer, "LOGIN"))
                            {
                                if(!auth)
                                {
                                    printf("Login wird ausgeführt\n");

                                    strcpy(buffer,"server ready");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error reading stuff");

                                    int i = 0;
                                    i = server_auth(new_socket, fails);
                                    if(!i)
                                    {
                                        printf("Login successful!\nClient ip: %s\n",inet_ntoa (cliaddress.sin_addr));
                                        auth = 1;
                                    }
                                    if(i == 3)
                                        fails ++;

                                    if(i == 4)
                                    {
                                        printf("Client closed remote socket\n");

                                        free(vdir);
                                        kill(pid, SIGHUP);
                                        exit(0);

                                    }

                                    if(fails==3)
                                    {
                                        perror("Client auth fail. Blocking Ip..\n");
                                        kill(pid, SIGTERM);



                                        memset(buffer,0,sizeof(buffer));
                                        sprintf(buffer,"%s",inet_ntoa (cliaddress.sin_addr));
                                        if(write(fd[1],buffer,BUF)==-1)
                                            perror("Error reading stuff");
                                        close(fd[1]);

                                        memset(buffer,0,sizeof(buffer));
                                        kill(pid, SIGHUP);
                                        _exit(1);
                                    }
                                }
                                else
                                {
                                    strcpy(buffer,"log");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error reading stuff");
                                }
                            }
                            else if(!strcasecmp(buffer, "PUT"))
                            {
                                if(auth)
                                {
                                    strcpy(buffer,"server ready");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error writing stuff");
                                    memset(buffer,'\0',sizeof(buffer));

                                    printf("Put wird ausgeführt\n");
                                    s_put(vdir, new_socket);
                                }
                                else
                                {
                                    strcpy(buffer,"log");
                                    if(write(new_socket,buffer,BUF)==-1)
                                        perror("Error writing stuff");
                                    memset(buffer,'\0',sizeof(buffer));
                                }
                            }
                            else if(!strcasecmp(buffer, "QUIT"))
                            {
                                printf("Client quit\n");

                            }
                            else
                            {
                                printf("The fuck dude? We shouldn't be here.. BUFFER: %s\n",buffer);
                            }
                        }
                    }
                    else if (size == 0)
                    {
                        printf("Client from %s closed remote socket\n",inet_ntoa (cliaddress.sin_addr));
                        free(vdir);
                        close (new_socket);
                        kill(pid, SIGHUP);
                        _exit(0);
                    }
                    else
                    {
                        perror("recv error");
                        return EXIT_FAILURE;
                    }
                }
                while (strcasecmp (buffer, "QUIT")  != 0);
                close (new_socket);
                kill(pid, SIGHUP);
                _exit(0);

            }
        }
        close (create_socket);
    }

    /* ENDE VERBINDUNG */
    free(vdir);

    return 0;
}


void checkdir(char* dir)
{
    DIR* dirp;


    /* Wird aktiviert wenn relative Pfadangabe möglich gemacht wird */
    {

        if(dir[0] != '/')
        {
            /*
               char cwd[PATH_MAX];
               getcwd(cwd,PATH_MAX);

               int sl = 0;

               printf("cwd: %s", cwd);

               char* token1 = NULL;
               char* token2 = NULL;
               char abPath[PATH_MAX] = "";

               token2 = strtok(dir,"/");
               token1 = strtok(cwd,"/");

               while(token1 != NULL)
               {
                   strcat(abPath,"/");
                   strcat(abPath,token1);

                   if(strcmp(token1,token2) == 0)
                   {
                       break;
                   }
                   token1 = strtok(NULL,"/");
               }
               dir = abPath;*/


        }

    }

//   printf("\nPfad: %s\n\n", dir);

    if((dirp = opendir(dir)) == NULL)
    {
        perror("Failed to open directory: ");
        printf("%s\n", dir);
        print_usage();
        return;
    }


    closedir(dirp);

}
