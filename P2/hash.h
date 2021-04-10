/**
 * @file hash.h
 * @brief Implementation of a hashtable of linked lists.
 */

#ifndef HASH_H
#define HASH_H

#include "list.h"

/**
 * Function declaration.
 */
list **init_tab();
void tab_add(list **tab,unsigned long i, node *n);
void dummy(void *n);
void free_hash(list **table);
unsigned long djb2(char *str);
unsigned long hash(char *str);

#endif