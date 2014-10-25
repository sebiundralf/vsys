#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUF 1024
char *s_gets(char *str, int n);

void print_usage()
{
    fprintf(stderr,"\nUsage: vsys_client -p PORT -i IP-ADRESS\n\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char* argv[])
{

    char* vip = NULL; //IP-Adresse des Servers, Variable muss später freigegeben werden
    int vport = -1; //Portnummer


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

    /* Verbindungsaufbau */
    {
        int create_socket;
        char buffer[BUF];
        struct sockaddr_in address;
        int size;
        char* command;
        char* file;

        if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Socket error");
            return EXIT_FAILURE;
        }

        memset(&address,0,sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons (vport);
        inet_aton(vip, &address.sin_addr);

        if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
        {
            printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
            size=recv(create_socket,buffer,BUF-1, 0);
            if (size>0)
            {
                buffer[size]= '\0';
                printf("%s",buffer);
            }
        }
        else
        {
            perror("Connect error - no server available");
            return EXIT_FAILURE;
        }

        do
        {
            printf ("Send command: ");
            s_gets(buffer, BUF);
           // char* ptr;
           char buf2[BUF];
           strcpy(buf2,buffer);

            command = strtok(buf2, " ");
            file = strtok(NULL, " ");

           // printf("Command = %s\n", command);
           // if(file!=NULL)
                //printf("File = %s\n", file);

            if(!strcasecmp(command, "LIST")){

                printf("List wird ausgeführt\n");

            }
            else if(!strcasecmp(command, "GET")){

                printf("Get wird ausgeführt\n");

            }
            else if(!strcasecmp(command, "SEND")){
                printf("Send wird ausgeführt\n");


            }
            else if(!strcasecmp(command, "QUIT")){
                printf("Client wird beendet\n");

            }
            else{
                printf("Unknown command: %s\n", command);

            }







            send(create_socket, buffer, strlen (buffer), 0);
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
