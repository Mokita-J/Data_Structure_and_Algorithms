/**
 * @file hash.c
 * @brief Implementation a of a hashtable of linked lists.
 */

#include <stdlib.h>

#include "list.h"
#include "hash.h"

/*'TABSIZE' is the size of the hashtable*/
#define TABSIZE 877


/**
 * @brief Function that initializes the hashtable.
 *
 * @return Pointer to the table.
 */
list **init_tab(){
    int i;
    list **table;
    table= malloc(sizeof(list)*TABSIZE);
    for (i=0; i<TABSIZE; i++){
        table[i]=NULL;
    }
    return table;
}


/**
 * @brief Function that doesn't do anything.
 */
void dummy(void *n){}


/**
 * @brief Function that frees the memory allocated for the hashtable.
 *
 * @param 'table' pointer to a hashtable.
 */
void free_hash(list **table){
    int i;
    for (i=0; i<TABSIZE; i++){
        if (table[i]){
            free_list(table[i], dummy);
        }
    }
    free(table);
}


/**
 * @brief Function that adds a node to the hashtable.
 *
 * @param 'tab' pointer to a hashtable.
 * @param 'i' index where the node will be added.
 * @param 'n' pointer to the node that is going to be added.
 */
void tab_add(list **tab,unsigned long i, node *n){
    if (tab[i]==NULL){
        tab[i]=init_list();
    }
    list_add(tab[i], n);
}


/**
 * @brief Hash function.
 *  This function uses the Djb2 Hash function.
 *
 * @return Index where the string belongs in the hashtable.
 */
unsigned long hash(char *str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash%TABSIZE;
}
