#include "server.h"


void clear_list(blacklist *head) //löscht alle einträge
{
    if(head==NULL)
        return;

    if(head->next == NULL)
    {
        free(head);
        return;
    }

    for(blacklist * current = head->next; current!=NULL; current=current->next)
    {
	free(head);
        head = current;
        
    }
    return;
}



void add_blacklist_entry(blacklist**head, struct in_addr ip) //fügt neuen Eintrag hinzu
{

    blacklist* new_entry = (blacklist*) malloc(sizeof(blacklist));

    //printf("hallo!");

    time_t t;

    time(&t);

    new_entry->sin_addr = ip;
    new_entry->t = t;
    new_entry->timestamp = localtime(&t);
    new_entry->next = NULL;

    if(*head == NULL)
    {
        *head = new_entry;
        printf("ip blocked: %s\n",inet_ntoa (new_entry->sin_addr));
        return;
    }

    blacklist * last;
    for(last = (*head); last->next ==NULL; last = last->next);
    last->next = new_entry;

    printf("ip blocked: %s\n",inet_ntoa (new_entry->sin_addr));

    return;

}

int check_blacklist(blacklist**head, struct in_addr ip) //Liefert die Sekunden bis zum aufheben der blockade zurück und löscht nebenbei abgelaufene blocks
{

    if((*head)==NULL)
        return 0;

    /* Check ob ips freigegebenwerden können */


    time_t t;
    int blocktimer = BLOCKTIME * 60;

    time(&t);
    int int_time = t;

    blacklist* last;
    blacklist* current;
    blacklist* delete_entry;

    if((*head)->next==NULL)
    {
        if((int)(*head)->t - int_time >= blocktimer)
        {
            free((*head));
            (*head) = NULL;
            return 0;
        }
    }
    else
    {
        while(1)
        {

            if((int)(*head)->t - int_time >= blocktimer)
            {
                delete_entry = (*head);
                (*head) = (*head)->next;
                free(delete_entry);

                if((*head) == NULL)
                    return 0;
            }
            else
            {
                break;
            }
        }

        if((*head)->next!=NULL)
        {

            last = *head;
            for(current=(*head)->next; current!=NULL; current =current->next)
            {


                if((int)current->t - int_time >= blocktimer)
                {
                    delete_entry = current;
                    last->next = last->next->next;
                    current = last;
                    free(delete_entry);

                }
                last=current;
            }

        }
    }
       /* Nach eintrag suchen */

    for(current=*head; current!=NULL; current =current->next)
        if(current->sin_addr.s_addr==ip.s_addr)
            return (BLOCKTIME*60)-(int_time - (int)current->t);


    return 0;




}



