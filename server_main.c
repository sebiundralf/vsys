#include "server.h"


/* Funktion print_usage() zur Ausgabe der usage Meldung */
void print_usage()
{
    fprintf(stderr,"\nUsage: vsys_server -p PORT -d DIRECTORY\n\n");
    exit(EXIT_FAILURE);
}



int main(int argc, char* argv[])

{
    char* vdir = NULL; //Verzeichnispfad, muss später freigegeben werden
    int vport = -1; //Portnummer


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


    /* ANFANG Verbindungsaufbau */
    {
        int create_socket, new_socket;
        socklen_t addrlen;
        char buffer[BUF];
        int size;
        struct sockaddr_in address, cliaddress;

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

        while (1)
        {
            printf("Waiting for connections...\n");
            new_socket = accept ( create_socket, (struct sockaddr *) &cliaddress, &addrlen );
            if (new_socket > 0)
            {
                printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr),ntohs(cliaddress.sin_port));
                strcpy(buffer,"Welcome to vsys_server, Please enter your command:\n");
                send(new_socket, buffer, strlen(buffer),0);
            }
            do
            {
                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0)
                {
                    buffer[size] = '\0';
                    //      printf ("Message received: %s\n", buffer);
                    /* Befehl auslesen */
                    {
                        if(!strcasecmp(buffer, "LIST"))
                        {

                            printf("List wird ausgeführt\n");
                            s_list(vdir, new_socket);

                        }
                        else if(!strcasecmp(buffer, "GET"))
                        {

                            printf("Get wird ausgeführt\n");
                            s_get(vdir, new_socket);

                        }
                        else if(!strcasecmp(buffer, "PUT"))
                        {
                            printf("Put wird ausgeführt\n");
                            s_put(vdir, new_socket);


                        }
                        else if(!strcasecmp(buffer, "QUIT"))
                        {
                            printf("Client quit\n");

                        }
                        else
                        {
                            printf("The fuck dude? We shouldn't be here..");
                        }
                    }
                }
                else if (size == 0)
                {
                    printf("Client closed remote socket\n");
                    break;
                }
                else
                {
                    perror("recv error");
                    return EXIT_FAILURE;
                }
            }
            while (strncasecmp (buffer, "QUIT")  != 0);
            close (new_socket);
        }
        close (create_socket);

    }
    /* ENDE VERBINDUNG */


    return 0;
}
