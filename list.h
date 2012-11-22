/**
 * iclc - Interactive Command Line Calculator
 *
 * arglist.h
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

#ifndef _ARGLIST_H_
#define _ARGLIST_H_

#include "main.h"

struct _ARGLIST_EL {
    char *name;
    struct _AST_NODE *val;

    struct _ARGLIST_EL *next;
};

struct _ARGLIST {
    struct _ARGLIST_EL *first;    
};

typedef struct _ARGLIST_EL arglist_el;
typedef struct _ARGLIST arglist;

arglist *arglist_create();
void arglist_add(arglist *l, const char *name, struct _AST_NODE *val);
const arglist_el *arglist_get_index(arglist *l, int i);
const arglist_el *arglist_get_name(arglist *l, const char *name);
void arglist_free(arglist *l);

#endif // _ARGLIST_H_
