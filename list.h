/**
 * iclc - Interactive Command Line Calculator
 *
 * list.h
 *
 * iclc is a simple, interactive, command line calculator. It
 * employs and thereby demonstrates basic parsing technique. 
 *
 * Beware, it leaks like a strainer and probably has more bugs
 * than Starship Troopers.
 *
 *
 * Copyright 2012 Patrick Haas (@p7haas)
 *
 * Released under the MIT and GPL licenses.
 *
 * ------------------------------------------------
 *  author:  Patrick Haas
 *  url:     https://github.com/p7haas/iclc
 *  source:  https://github.com/p7haas/iclc
 */

#ifndef _LIST_H_
#define _LIST_H_

#include "main.h"

#define DECLARE_LIST(TYPE)          \
typedef struct TYPE##_el_tag {      \              
    struct TYPE##_el_tag *prev;     \
    struct TYPE##_el_tag *next;     \
                                    \
    TYPE data;                      \
} TYPE##_el_t;                      \
                                    \
struct TYPE##_list_tag {            \
    TYPE##_el_t **first;            \
    TYPE##_el_t **last;             \
    unsigned long size;             \
} TYPE##list_t;



typedef struct _ARGLIST_EL arglist_el;
typedef struct _ARGLIST arglist;

arglist *arglist_create();
void arglist_add(arglist *l, const char *name, struct _AST_NODE *val);
const arglist_el *arglist_get_index(arglist *l, int i);
const arglist_el *arglist_get_name(arglist *l, const char *name);
void arglist_free(arglist *l);

#endif // _ARGLIST_H_
