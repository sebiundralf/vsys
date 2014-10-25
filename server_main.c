#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

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




return 0;
}
