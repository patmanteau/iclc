/**
 * iclc - Interactive Command Line Calculator
 *
 * store.c
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

#include "store.h"

void store_var(store *st, const char *name, double val) {
    var_item **it = &st->__first;
    
    while (1==1) {
        // neuer Eintrag?
        if (*it == NULL) {
            var_item *new_item = malloc(sizeof(var_item));
            new_item->name = calloc(strlen(name)+1, sizeof(char));
            strcpy(new_item->name, name);
            new_item->val = val;
            new_item->__next = NULL;
            *it = new_item;
            return;
        } else if (strcmp((*it)->name, name) == 0) {
            (*it)->val = val;
            return;
        } else it = &(*it)->__next;
    }
}

bool store_has_var(store *st, const char *name) {
    var_item *it = st->__first;
    while (it != NULL) {
        if (strcmp(it->name, name) == 0) return true;
        it = it->__next;
    }
    return false;
}

double store_get_var(store *st, const char *name) {
    var_item *it = st->__first;
    while (it != NULL) {
        if (strcmp(it->name, name) == 0) return it->val;
        it = it->__next;
    }
    return false;
}

store *store_create() {
    store *st = malloc(sizeof(store));
    st->__first = NULL;
    return st;
}

void store_free(store *st) {
    var_item *it = st->__first;
    var_item *n;
    while (it != NULL) {
        n = it->__next;
        free(it->name);
        free(it);
        it = n;
    }
    st->__first = NULL;
}

store *store_get() {
    static store *the_store = NULL; 

    if (the_store == NULL) the_store = store_create();
    return the_store;
}
