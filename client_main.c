#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

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



    return 0;
}
