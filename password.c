#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>



void get_password(char* pw, int l)  //Chararray kund max länge (-1 für \0) übergeben
{
        char passwd[l];
        char *in = passwd;
        struct termios  tty_orig;
        char c;
        tcgetattr( STDIN_FILENO, &tty_orig );
        struct termios  tty_work = tty_orig;

        puts("Please input password:");
        tty_work.c_lflag &= ~( ECHO | ICANON );  // | ISIG );
        tty_work.c_cc[ VMIN ]  = 1;
        tty_work.c_cc[ VTIME ] = 0;
        tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_work );

         for(int i = 0;i<l;i++) {
                if (read(STDIN_FILENO, &c, sizeof c) > 0) {
                        if ('\n' == c) {
                                break;
                        }
                        *in++ = c;
                        if(write(STDOUT_FILENO, "*", 1)==-1)
                            printf("PW entering fail\n");
                }
        }

        tcsetattr( STDIN_FILENO, TCSAFLUSH, &tty_orig );

        *in = '\0';
        fputc('\n', stdout);

        // if you want to see the result:
        // printf("Got password: %s\n", passwd);

        strcpy(pw, passwd);
        return;



}
