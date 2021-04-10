/*
 * File: proj1.c
 * Author: Monica Chen Jin al92532
 * Description: Project 1 of IAED. This program schedules events.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXIN 350       /* Maximum characters' input in a line*/
#define MAXCHAR 64      /* 63 + '\0' characters stored in a name*/
#define MDATE 9         /* Number of characters for the date*/
#define MTIME 5         /* Number of characters for the time*/
#define MROOM 3         /* Number of characters for the room*/
#define MEVENT 1001     /* Array has 1001 because variable "size" indicates the next free space*/
#define MAXPPL 4        /* Maximum number of people in a event*/
#define MINPPL 2        /* Mininum number of people in a event*/
#define TRUE 1
#define FALSE 0

/* Structure that represents a date:
 * str- string of the input date; num- integer of date in format YYYYMMDD
*/
typedef struct
{
    char str[MDATE];
    long num;
}date;

/* Structure that represents time:
 * str- sting of the input time; num- time in minutes
*/
typedef struct
{
    char str[MTIME];
    int num;
}time;

/* Strtucture that represents an event*/
typedef struct
{
    char desc[MAXCHAR];
    date day;
    time start;
    int span;
    int room;
    char ppl[MAXPPL][MAXCHAR];
    int pplnum;
}event;


/* Declaration and Initialization of local variables to the program*/
static event e[MEVENT];    /* Array of events that stores all of the events*/
static int size=0;         /* The next free index of array e[] & the number of events stored*/


/* Functions used in the program*/
long str_date(char str[MDATE]);
int str_time(char str[MTIME]);
void ini_ppl(char str[MAXPPL][MAXCHAR]);
int same_room(int r1, int r2);
int same_day(date d1, date d2);
int same_time(event e1, event e2);
int available_space(event e1, int n);
int same_ppl(char p1[MAXCHAR], char p2[MAXCHAR]);
int available_ppl(event e1, int n, char cmd);
int more(event test, event key);
void insertion_sort(int first);
void add_event(char buffer[MAXIN]);
void list_all();
void list_room(int room);
int event_exists(char buffer[MAXIN]);
void rm_event(char buffer[MAXIN]);
void change_time(char buffer[MAXIN]);
void change_span(char buffer[MAXIN], int span);
void change_room(char buffer[MAXIN], int room);
int in_event(char person[MAXCHAR], event e1);
void add_ppl(char buffer[MAXIN]);
void rm_ppl( char buffer[MAXIN]);


/* Reads the input command and executes it*/
int main(){
    int cmd, num;
    char buffer[MAXIN];

    while((cmd=getchar())!= EOF){
        getchar();
        switch (cmd){
            case 'a':
                fgets(buffer, MAXIN, stdin);
                add_event(buffer);
                break;

            case 'l':
                list_all();
                break;
            
            case 's':
                scanf("%d", &num);
                getchar();
                list_room(num);
                break;
            case 'r':
                scanf("%[^\n]", buffer);
                getchar();
                rm_event(buffer);
                break;
            case 'i':
                fgets(buffer, MAXIN, stdin);
                change_time(buffer);
                break;
            case 't':
                scanf("%[^:]", buffer);
                getchar();
                scanf("%d", &num);
                getchar();
                change_span(buffer, num);
                break;

            case 'm':
                scanf("%[^:]", buffer);
                getchar();
                scanf("%d", &num);
                getchar();
                change_room(buffer, num);
                break;
            
            case 'A':
                fgets(buffer, MAXIN, stdin);
                add_ppl(buffer);
                break;
            
            case 'R':
                fgets(buffer, MAXIN, stdin);
                rm_ppl(buffer);
                break;

            case 'x':
                return EXIT_SUCCESS;

            default:    /*in regular situations this case will never be reached*/
                fprintf(stderr, "Invalid Input!\n");
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}


/* Converts a given date in characters to a integer in the format YYYYMMDD*/
long str_date(char str[MDATE]){
    long num;
    int month, year;
    num= atol(str);
    year= num%10000;
    month= (num/10000)%100;
    num = num/1000000 + month*100 + year*10000;
    return num;
}

/* Converts a given time in characters to a integer of minutes*/
int str_time(char str[MTIME]){
    int num;
    num = atoi(str);
    num = (num/100)*60 + num%100;
    return num;
}

/* Initializes all of possible people in the event*/
void ini_ppl(char str[MAXPPL][MAXCHAR]){
    int i;
    for (i=0; i<= MAXPPL; i++){
        str[i][0]= '\0';
         
    }
}

/* Compares two given rooms*/
int same_room(int r1, int r2){
    if (r1 == r2) return TRUE;
    else {return FALSE;}
}

/* Compares two given dates*/
int same_day(date d1, date d2){
    if (d1.num == d2.num) return TRUE;
    else {return FALSE;}
}

/* Verifies if two given events overlap*/
int same_time(event e1, event e2){
    int end1, end2;
    end1= e1.start.num + e1.span -1;
    end2= e2.start.num + e2.span -1;
    if ((e2.start.num > end1) || (end2< e1.start.num))
        return FALSE;
    else {return TRUE;}
}

/* Verifies the availability of a room*/
int available_space(event e1, int n){
    int i;
    for (i=0; i<size; i++){
        if (i==n){
            continue;
        }
        if (same_room(e1.room, e[i].room) && same_day(e1.day, e[i].day) && same_time(e1, e[i])){
            printf("Impossivel agendar evento %s. Sala%d ocupada.\n", e1.desc, e[i].room);
            return FALSE;
        }
    }
    return TRUE;
}

/* Compares two given names*/
int same_ppl(char p1[MAXCHAR], char p2[MAXCHAR]){
    if (!strcmp(p1, p2)) return TRUE;
    else {return FALSE;}
}

/* Verifies if the people involved in the event are available*/
int available_ppl(event e1, int n, char cmd){
    int i, j, k, flag= TRUE;
    for (i=0; i<size; i++){
        if (i==n){
            continue;
        }
        if (same_day(e1.day, e[i].day) && same_time(e1, e[i])){
            for (j=0; j< e1.pplnum; j++){
                for (k=0; k<e[i].pplnum; k++){
                    if (same_ppl(e1.ppl[j], e[i].ppl[k])){
                        if (cmd=='A'){
                            printf("Impossivel adicionar participante. ");
                        }
                        else {
                            printf("Impossivel agendar evento %s. ", e1.desc);
                        }
                    printf("Participante %s tem um evento sobreposto.\n", e[i].ppl[k]);
                    flag = FALSE;
                    }
                }
            }
        }
    }
    return flag;
}

/* Compares two events' time and room number*/
int more(event test, event key){
    if (test.day.num > key.day.num) return TRUE;
    else if (test.day.num==key.day.num){
        if (test.start.num>key.start.num){
            return TRUE;
        }
        else if (test.start.num== key.start.num && test.room> key.room){
                return TRUE;
        }
    }
    return FALSE;
}

/* Sorts a given event to its right place in array e[]*/
void insertion_sort(int first){
    int i, j;
    event key;
    for (i=first; i<size; i++){
        key= e[i];
        j= i-1;
        while (j>=0 && more(e[j], key)){
            e[j+1] = e[j];
            j-=1;
        }
        e[j+1]= key;
    }
}


/* Adds a event into array e[]*/
void add_event(char buffer[MAXIN]){
    int i=0;
    char *token;
    token = strtok(buffer, ":");
    strcpy(e[size].desc, token);
    token= strtok(NULL, ":");
    strcpy(e[size].day.str, token);
    e[size].day.num= str_date(token);
    token= strtok(NULL, ":");
    strcpy(e[size].start.str, token);
    e[size].start.num= str_time(token);
    token= strtok(NULL, ":");
    e[size].span= atoi(token);
    token= strtok(NULL, ":");
    e[size].room= atoi(token);
    token= strtok(NULL, ":");
    ini_ppl(e[size].ppl);
    while(token!=NULL){
        strcpy(e[size].ppl[i], token);
        i++;
        token= strtok(NULL, ":\n");
    }
    e[size].pplnum= i;              /* i is the free index of ppl array*/
    if (size==0 || available_space(e[size],size)){
            if (available_ppl(e[size], size, 'a')){
                size++;             /* the size increments only when the conditions are met*/        
                insertion_sort(size-1);
            }
        }
}

/* Lists all of the events in cronologic order*/
void list_all(){
    int i,j;
    for (i=0; i<size; i++){
        printf("%s %s %s %d Sala%d %s\n*",
        e[i].desc, e[i].day.str, e[i].start.str, e[i].span, e[i].room, e[i].ppl[0]);
        for (j=1; j<e[i].pplnum; j++){
            printf(" %s",e[i].ppl[j]);
        }
        printf("\n");
    }
}

/* Lists all of the events in a given room*/
void list_room(int room){
    int i, j;
    for (i=0; i<size; i++){
        if (e[i].room==room){
            printf("%s %s %s %d Sala%d %s\n*",
            e[i].desc, e[i].day.str, e[i].start.str, e[i].span, e[i].room, e[i].ppl[0]);
            for (j=1; j<e[i].pplnum; j++){
                printf(" %s",e[i].ppl[j]);
            }
            printf("\n");
        }
    }
}

/* Verifies if the given description is in the array e[]*/
int event_exists(char buffer[MAXIN]){
    int i;
    for (i=0; i<size; i++){
        if (strcmp(buffer,e[i].desc)==0)
            return TRUE+i;  /*returns the matching description event's index + 1(TRUE)*/
    }
    printf("Evento %s inexistente.\n", buffer);
    return FALSE;
}

/* Removes the given description's event form the array e[]*/
void rm_event(char buffer[MAXIN]){
    int n;
    n= event_exists(buffer);
    if (n) {
        n-= TRUE;           /*obtains event's index*/
        for(; n<size-1; n++){
            e[n]= e[n+1];
        }
        size--;
    }
}

/* Changes the starting time of the given event*/
void change_time(char buffer[MAXIN]){
    int n;
    char *token, start0[MTIME];
    token = strtok(buffer, ":");
    n = event_exists(token);
    if (n){
        n-=TRUE;        /* obtains event's index*/
        strcpy(start0, e[n].start.str);
        token = strtok(NULL, ":\n");
        strcpy(e[n].start.str, token);
        e[n].start.num= str_time(token);
        if (available_space(e[n], n) && available_ppl(e[n], n, 'i'))
                insertion_sort(n);
        else {          /* when the conditions fail, the original data is restored*/
             strcpy(e[n].start.str, start0);
             e[n].start.num = str_time(start0);
        }
    }
}

/* Changes the time span of the given event*/
void change_span(char buffer[MAXIN], int span){
    int n;
    int span0;
    n= event_exists(buffer);
    if (n) {
        n-=TRUE;
        span0= e[n].span;
        e[n].span= span;
        if (!available_space(e[n], n) || !available_ppl(e[n], n, 't'))
            e[n].span=span0; /* when the conditions fail, the original time span is restored*/
    }
}

/* Changes the room number of the given event*/
void change_room(char buffer[MAXIN], int room){
    int n;
    int room0;
    n= event_exists(buffer);
    if (n) {
        n-=TRUE;
        room0= e[n].room;
        e[n].room= room;
        if (available_space(e[n], n) && available_ppl(e[n], n, 'm'))
                insertion_sort(n);
        else{
            e[n].room=room0; /* the original room number is restored*/
        }
    }
}

/* Verifies if a given person is in a given event*/
int in_event(char person[MAXCHAR], event e1){
    int i;
    for (i=0; i<MAXPPL; i++){
        if (!strcmp(person, e1.ppl[i])){
            return TRUE;
        }
    }
    return FALSE;
}

/* Adds a person to a given event*/
void add_ppl(char buffer[MAXIN]){
    int n, i;
    char *token, name[MAXCHAR];
    token = strtok(buffer, ":");
    n = event_exists(token);
    if (n) {
        n-= TRUE;       /*obtains event's index*/
        token = strtok(NULL, ":\n");
        strcpy(name, token);
        i =e[n].pplnum; /*indicates the next free space in array ppl & the number of ppl in the event*/
        if (!in_event(name, e[n])){
            if (i==MAXPPL){
                printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", e[n].desc);
            }
            else {
                strcpy(e[n].ppl[i], name);
                e[n].pplnum++;
                if (!available_ppl(e[n], n, 'A')){
                    e[n].ppl[i][0]= '\0';   /*if the added person isn't available, the original data is restored*/
                    e[n].pplnum--;
                }
            }
        }
    }
}

/* Removes a person from a given event*/
void rm_ppl( char buffer[MAXIN]){
    int i, n, rmindex=0;
    char *token, name[MAXCHAR];
    token = strtok(buffer, ":");
    n = event_exists(token);
    if (n) {
        n-= TRUE;                 /*obtains the event's index*/
        token = strtok(NULL, ":\n");
        strcpy(name, token);
        if (e[n].pplnum==MINPPL && in_event(name, e[n])){
            printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n",e[n].ppl[1], e[n].desc);
        }
        else if (in_event(name, e[n]) && (!same_ppl(name, e[n].ppl[0]))){ /*if name is in the event & isn't the person in charge*/
            for (i=1; i< e[n].pplnum; i++){ /*starts with 1 because the person with index 0 cant be removed*/
                if (same_ppl(name, e[n].ppl[i])){
                    rmindex=i;
                    break; /*once the index we want to remove is found this loop ends*/
                }
            }
            for(i = 0; i < e[n].pplnum-rmindex-1; i++)
            {
                strcpy(e[n].ppl[rmindex+i], e[n].ppl[rmindex+1+i]);
            }
            e[n].pplnum--;
        }
    }
}