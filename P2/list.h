/**
 * @file list.h
 * @brief Implementation of a doubly linked list.
 */

#ifndef LIST_H
#define LIST_H

/**
 * @brief Structure that represents a node of a doubly linked list.
 */
typedef struct stru_node{
    void *info;
    struct stru_node *previous, *next;
}node;


/**
 * @brief Structure that represents a doubly linked list.
 */
typedef struct stru_list{
    struct stru_node *head;
    struct stru_node *last;
}list;

/**
 * Function declaration.
 */
list* init_list();
node* mknode(void *p);
void list_add(list *l,node *n);
void list_remove(list *l, node *n);
void free_list(list *l, void (*freeInfo)(void*));

#endif