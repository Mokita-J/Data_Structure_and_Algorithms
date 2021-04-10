/**
 * @file list.c
 * @brief Implementation of a doubly linked list.
 */
#include <stdlib.h>
#include "list.h"


/**
 * @brief Function that creates a list.
 *
 * @return Pointer to the created list.
 */
list* init_list(){
    list *l;
    l= (list*)malloc(sizeof(list));
    l->head=l->last=NULL;
    return l;
}

/**
 * @brief Function that creates a node.
 *
 * @param 'p' any kind of pointer.
 *
 * @return Pointer to the created node.
 */
node* mknode(void *p){
    node *n=malloc(sizeof(node));
    n->next=n->previous=NULL;
    n->info=p;
    return n;
}

/**
 * @brief Function that adds a node to the end of a list.
 *
 * @param 'l' pointer to a list.
 * @param 'n' pointer to a node.
 */
void list_add(list *l,node *n){
    n->previous=l->last;
    if (l->last)
        l->last->next=n;
    else l->head=n;
    l->last=n;
}

/**
 * @brief Funtion that removes a node from a list.
 *
 * @param 'l' pointer to a list.
 * @param 'n' pointer to the node that is going to be removed.
 */
void list_remove(list *l, node *n){
    if(n->previous)
            n->previous->next=n->next;
        else
            l->head=n->next;
        if (n->next)
            n->next->previous=n->previous;
        else
            l->last=n->previous;
        free(n);
}


/**
 * @brief Function that frees the memory allocated for a list.
 *
 * @param 'l' pointer to a list.
 * @param 'freeInfo' function pointer that frees the memory allocated.
 */
void free_list(list *l, void (*freeInfo)(void*)){
    node *aux= l->head, *tmp;
    while (aux){
        (*freeInfo)(aux->info);
        tmp=aux;
        aux=aux->next;
        free(tmp);
    }
    free(l);
}
