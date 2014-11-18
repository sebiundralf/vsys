#include "client.h"


int login = 0;

void print_usage()
{
    fprintf(stderr,"\nUsage: vsys_client -p PORT -i IP-ADRESS\n\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char* argv[])
{

    char* vip = NULL; //IP-Adresse des Servers, Variable muss später freigegeben werden
    int vport = -1; //Portnummer
    // int error = 0; //nur temorär verwendet zum debugging
    memset(username,'\0',sizeof(username));
    /* Start der GETOPT behandlung */
    {
        int c;
        int counter = 0;
        int sl = 0;

        if(argc <3)
            print_usage();

        while( (c = getopt(argc, argv, "i:p:")) != EOF )
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

            case 'i':
                if(vip!=NULL)
                {
                    print_usage();
                    break;
                }
                sl = strlen(optarg);
                vip = (char*) malloc(sizeof(char)* (sl+1));
                strcpy(vip, optarg);
                counter++;
                break;

            case '?':
                print_usage();
                break;
            }
        }

        // printf("\nIP: %s, Port: %d\n",vip, vport);

        // printf("\nCounter: %d; optind: %d; argc: %d\n", counter, optind, argc);

        if(counter!= 2 || optind!=argc || vport < 1)
            print_usage();


        //printf("\n FERTIG\n ");
        /* ENDE GETOPT */
    }

    /* Kommunikation mit server */
    {
        int create_socket;
        char buffer[BUF];
        struct sockaddr_in address;
        char* command;
        char* file;

        memset(buffer,'\0',sizeof(buffer));

        /* Socket erstellen */
        {
            if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
            {
                perror("Socket error");
                return EXIT_FAILURE;
            }

            memset(&address,0,sizeof(address));
            address.sin_family = AF_INET;
            address.sin_port = htons (vport);
            inet_aton(vip, &address.sin_addr);
            free(vip);
        }
        /* connect */
        {
            if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
            {
                clrscr();

                do
                {
                    if(read(create_socket,buffer,BUF)==-1)
                        perror("Error reading stuff");

                    if(!strcmp(buffer,"fail"))
                    {
                        perror("Server failed, shutting down client...");
                        close (create_socket);
                        return EXIT_FAILURE;
                    }

                    if(!strncasecmp(buffer,"block",5))
                    {
                        strtok(buffer, "|");
                        char* printmsg = strtok(NULL, "|");
                        printf("\n%s\nShutting down client..\n",printmsg);
                        close(create_socket);
                        return EXIT_FAILURE;
                    }
                }
                while(strcmp(buffer,"win"));

                printf ("Connection with server (%s) established\nPlease log in first with comand: \"LOGIN\"\n", inet_ntoa (address.sin_addr));

            }
            else
            {
                perror("Connect error - no server available");
                return EXIT_FAILURE;
            }
        }


        do
        {
            login = 0;
            printf ("Send command: ");
            s_gets(buffer, BUF);

            /* String trennen */
            {
                char buf2[BUF];
                strcpy(buf2,buffer);

                command = strtok(buf2, " ");
                file = strtok(NULL, "\n");

                // printf("Command: %s\nFile: %s\nbuf2: %s\n\n",command,file,buf2);

            }
            // printf("Command = %s\n", command);
            // if(file!=NULL)
            //printf("File = %s\n", file);

            /* Befehl ausführen */
            {
                if(!strcasecmp(command, "LIST"))
                {


                    if(send(create_socket, command, strlen (command), 0)==-1)
                        perror("Error sending stuff");

                    do
                    {
                        if(read(create_socket,buffer,BUF)==-1)
                            perror("Error reading stuff");

                        if(!strcmp(buffer,"log"))
                        {

                            //   printf("Server refused, login first\n");
                            login=1;
                            break;

                        }
                    }
                    while(strcmp(buffer,"server ready"));

                    if(!login)
                    {
                        printf("List wird ausgeführt\n\nListe:\n");

                        memset(buffer,'\0',sizeof(buffer));
                        {

                            strcpy(buffer,"start");
                            if(write(create_socket,buffer,BUF)==-1)
                                perror("Error writing stuff");

                            memset(buffer,'\0',sizeof(buffer));
                        }

                        do
                        {
                            if(read(create_socket,buffer, BUF)==-1)
                                perror("Error writing stuff");
                            //buffer[BUF-1] = '\0';
                            printf("%s",buffer);
                            /*  if(!strcmp(buffer,"list"))
                              {
                                  perror("\”Server error, shut down client\n");
                                  return EXIT_FAILURE;

                              }*/

                        }
                        while (strlen(buffer)!=0);
                        printf("\n");

                    }
                    else
                    {
                        printf("Error in LIST, you must login first, send command: \"LOGIN\"\n\n");
                    }

                }
                else if(!strcasecmp(command, "GET"))
                {
                   if(strstr(file, ".."))
                        printf("Wrong filename, filename must not contain \"..\"\n");
                    else
                        c_get(create_socket, file);
                }
                else if(!strcasecmp(command, "PUT"))
                {
                    c_put(create_socket, file);
                }
                else if(!strcasecmp(command, "LOGIN"))
                {
                    //printf("Put wird ausgeführt\n");

                    if(send(create_socket, command, strlen (command), 0)==-1)
                        perror("Error sending stuff");

                    do
                    {
                        if(read(create_socket,buffer,BUF)==-1)
                            perror("Error reading stuff");

                        if(!strcmp(buffer,"log"))
                        {
                            login = 1;
                            break;
                        }
                    }

                    while(strcmp(buffer,"server ready"));
                    if(!login)
                    {

                        clrscr();
                        client_auth(create_socket);

                    }
                    else
                    {
                        printf("Login refused\nAlready logged in as: %s\n\n",username);
                    }
                }
                else if(!strcasecmp(command, "QUIT"))
                {
                    printf("Client wird beendet\n");
                }
                else
                {
                    printf("Unknown command: %s\n\n", command);
                    //error = 1;
                }
            }
        }
        while (strcasecmp (command, "QUIT") != 0);
        close (create_socket);

    }
    /* Ende Verbindung */
    return 0;
}

char *s_gets(char *str, int n)
{

    char *ret_val;
    int i=0;
    ret_val=fgets(str, n, stdin);
    if(ret_val)
    {

        while (str[i] != '\n' && str[i] !='\0')
            i++;

        if(str[i] == '\n')
            str[i]='\0';
        else
            while(getchar()!='\n')
                continue;

    }

    return ret_val;

}
