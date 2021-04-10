/**
 * @author Monica Chen Jin al92532
 * @file main.c
 * @brief Implementation a programm that manages contacts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "hash.h"

/*Maximum number of chars in a name string*/
#define MAXNAME 1024

/*Maximum number of chars in a email string*/
#define MAXMAIL 512

/*Maximum number of chars in a nubmber string*/
#define MAXNUMB 64


/**
 * @brief Structure that represents a contact.
 */
typedef struct stru_contact{
    char *name, *local, *domain, *number;
}contact;


/**
 * Program's local variables.
 * 'l' is the list with all of the stored contacts.
 * 'nameTab' & 'domainTab' are hashtables used to store.
 * node addresses from 'l'.
 * 'nameTab' is indexed by the contact's name.
 * 'domainTab' is indexed by the contact's domain.
 */
static list *l, **nameTab, **domainTab;


/**
 * Function declaration.
 */
contact* mkcontact();
void freeContact(void *p);
node* search_name(list *l, char *name);
node* search_domain(list *l, contact *person);
void add_contact();
void print_list();
void search();
void remove_contact();
void change_domain();
void count_domain();


/**
 * @brief Function main.
 *
 * @return Program exit state.
 */
int main(){
    int cmd;

    l=init_list();
    nameTab= init_tab();
    domainTab= init_tab();

    while ((cmd=getchar())!=EOF && cmd!='x'){
        if (cmd=='\n') continue;
        switch (cmd)
        {
        case 'a':
            add_contact();
            break;
        case 'l':
            print_list();
            break;
        case 'p':
            search();
            break;
        case 'r':
            remove_contact();
            break;
        case 'e':
            change_domain();
            break;
        case 'c':
            count_domain();
            break;
        default:
            fprintf(stderr, "Invalid Input!\n");
            free_hash(domainTab);
            free_hash(nameTab);
            free_list(l, freeContact);
            return EXIT_FAILURE;
        }
    }
    free_hash(domainTab);
    free_hash(nameTab);
    free_list(l, freeContact);
    return EXIT_SUCCESS;
}


/**
 * @brief Function that creates a conctact.
 *
 * @return Pointer to the created contact.
 */
contact* mkcontact(){
    contact *info= malloc(sizeof(contact));
    return info;
}


/**
 * @bried Function that frees the memory allocated for the contact.
 *
 * @param 'p' pointer to a contact.
 */
void freeContact(void *p){
    contact *info=p;
    free(info->name);
    free(info->local);
    free(info->domain);
    free(info->number);
    free(p);
}


/**
 * @brief Function that searches a name in a given list.
 *
 * @param 'l' pointer to a list.
 * @param 'name' pointer to a string.
 *
 * @return Pointer to the node containing the searched name or
 *         NULL if the name doesnt't exist.
 */
node* search_name(list *l, char *name){
    node *tmp, *plist;
    contact *info;

    if (l==NULL)
        return NULL;

    for(tmp=l->head; tmp; tmp=tmp->next){

        /*pointer to the node in the list of contacts*/
        plist=tmp->info;

        /*pointer to the contact*/
        info=plist->info;

        if(strcmp(info->name, name)==0)
            return tmp;
    }
    return NULL;
}


/**
 * @brief Function that searches a domain in a given list.
 *
 * @param 'l' pointer to a list.
 * @param 'person' pointer to a contact.
 *
 * @return Pointer to the node containing the domain or
 *         NULL if the domain doesn't exist.
 */
node* search_domain(list *l, contact *person){
    node *tmp, *plist;
    contact *info;

    if (l==NULL)
        return NULL;

    for(tmp=l->head; tmp; tmp=tmp->next){

        /*pointer to the node in the list of contacts*/
        plist=tmp->info;

        /*pointer to the contact*/
        info=plist->info;

        if(!strcmp(info->domain, person->domain)
        && !strcmp(info->name, person->name))
            return tmp;
    }
    return NULL;
}


/**
 * @brief Function that adds a contact to the programm.
 */
void add_contact(){
    char *token, name[MAXNAME], email[MAXMAIL], number[MAXNUMB];
    contact *person;
    node *nlist, *n_name, *n_domain;
    unsigned long iname, idomain;

    scanf("%s %s %s", name, email, number);
    iname=hash(name); /*index of the hashtable organized by names*/

    if (search_name(nameTab[iname], name)){
        printf("Nome existente.\n");
        return;
    }

    person=mkcontact(); /*makes a new contact*/

    /*sizeof(char) is 1 so its use in malloc() it's redundant*/
    person->name= malloc(strlen(name)+1);
    strcpy(person->name, name);
    token=strtok(email, "@");
    person->local=malloc(strlen(token)+1);
    strcpy(person->local, token);
    token=strtok(NULL, "\0");
    person->domain=malloc(strlen(token)+1);
    strcpy(person->domain, token);
    person->number=malloc(strlen(number)+1);
    strcpy(person->number, number);

    /*makes a node to add to the list*/
    nlist=mknode(person);
    list_add(l,nlist);

    /*makes a node to add to the name hastable*/
    n_name=mknode(nlist);
    tab_add(nameTab,iname , n_name);

    /*makes a node to add to the domain hashtable*/
    n_domain=mknode(nlist);
    idomain=hash(person->domain);
    tab_add(domainTab, idomain, n_domain);
}

/**
 * @brief Function that prints all of the contacts in the stored order.
 */
void print_list(){
    node *aux;
    contact* info;

    for(aux= l->head; aux; aux=aux->next){
        info= aux->info;
        printf("%s %s@%s %s\n",info->name, info->local,
                               info->domain, info->number);
    }
}


/**
 * @brief Function that searches a name and
 *        prints the data associated to it.
 */
void search(){
    char name[MAXNAME];
    unsigned long i;
    node *ntab, *nlist;
    contact *info;

    scanf("%s", name);

    i=hash(name);
    ntab=search_name(nameTab[i], name);

    /*if the searched name is stored in the program*/
    if (ntab){
        nlist=ntab->info;
        info =nlist->info;
        printf("%s %s@%s %s\n", info->name, info->local,
                                info->domain, info->number);
    }
    else
        printf("Nome inexistente.\n");
}


/**
 * @brief Funtion that removes a contact from the program.
 */
void remove_contact(){
    char name[MAXNAME];
    unsigned long iname, idomain;
    node *nlist, *n_name, *n_domain;
    contact *info;

    scanf("%s", name);

    iname=hash(name);
    n_name=search_name(nameTab[iname], name);

    /*if the to be removed contact's name exists in the program*/
    if (n_name){
        nlist=n_name->info;
        info=nlist->info;
        idomain=hash(info->domain);
        n_domain=search_domain(domainTab[idomain], info);

        freeContact(info);
        list_remove(domainTab[idomain], n_domain);
        list_remove(nameTab[iname], n_name);
        list_remove(l, nlist);
    }
    else
        printf("Nome inexistente.\n");
}


/**
 * @brief Function that changes the domain of a contact.
 */
void change_domain(){
    unsigned long i;
    char *token, name[MAXNAME], email[MAXMAIL];
    node *nlist, *ntab, *dtab;
    contact *info;

    scanf("%s%s", name, email);
    i= hash(name);
    ntab=search_name(nameTab[i], name);

    /*if the to be changed contact is stored in the program*/
    if (ntab){
        token=strtok(email, "@");
        token=strtok(NULL, "\0");

        /*pointer to the node in the list of contacts*/
        nlist= ntab->info;

        /*pointer to the contact*/
        info=nlist->info;

        /*removes the contact from the domain hashtable*/
        i=hash(info->domain);
        dtab= search_domain(domainTab[i], info);
        list_remove(domainTab[i], dtab);

        /*changes the email address*/
        if (strlen(info->local)!=strlen(email)){
            free(info->local);
            info->local=malloc(strlen(email)+1);
        }
        strcpy(info->local, email);
        if (strlen(info->domain)!=strlen(token)){
            free(info->domain);
            info->domain=malloc(strlen(token)+1);
        }
        strcpy(info->domain, token);

        /*makes a node with the changed contact*/
        dtab=mknode(nlist);

        /*adds it to the domain hashtable*/
        i=hash(info->domain);
        tab_add(domainTab,i, dtab);
    }
    else
        printf("Nome inexistente.\n");
}


/**
 * @brief Function that counts all of the domains in the program
 *        that are equal to the given in the standart input.
 */
void count_domain(){
    unsigned long i;
    char domain[MAXMAIL];
    int count=0;
    node *tmp, *nlist;
    contact *info;
    scanf("%s", domain);
    i=hash(domain);
    if (domainTab[i]){
        for(tmp=domainTab[i]->head; tmp; tmp=tmp->next){

            /*pointer to a node in the list of contacts*/
            nlist= tmp->info;

            /*pointer to the contact*/
            info=nlist->info;

            if (!strcmp(info->domain, domain))
                count++;
        }
    }
    printf("%s:%d\n",domain, count);
}
