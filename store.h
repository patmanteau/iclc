/**
 * iclc - Interactive Command Line Calculator
 *
 * store.h
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

#ifndef _STORE_H_
#define _STORE_H_

#include "main.h"

struct _VAR_ITEM {
    char *name;
    double val;

    struct _VAR_ITEM *__next;
};

struct _STORE {
    struct _VAR_ITEM *__first;
};

typedef struct _VAR_ITEM var_item;
typedef struct _STORE store;

void store_var(store *st, const char *name, double val);

bool store_has_var(store *st, const char *name);
double store_get_var(store *st, const char *name);

store *store_get();
void store_free(store *st);

#endif // _STORE_H_
