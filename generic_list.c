/**
 * iclc - Interactive Command Line Calculator
 *
 * arglist.c
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

#include "arglist.h"
#include "snippets.h"

arglist *arglist_create() {
    arglist *l = safe_malloc(sizeof(arglist));
    l->first = NULL;

    return l;
}

void arglist_add(arglist *l, const char *name, struct _AST_NODE *val) {
    arglist_el **el = &l->first;
    while ((*el) != NULL) {
        el = &((*el)->next);
    }
    *el = safe_malloc(sizeof(arglist_el));
    (*el)->name = stringcopy(name);
    (*el)->val = val;
    (*el)->next = NULL;
}
    
int arglist_get_size(arglist *l) {
    arglist_el *el = l->first;
    int c = 0;
    while (el != NULL) {
        c++;
    }
    return c;
}

const arglist_el *arglist_get_index(arglist *l, int idx) {
    arglist_el *el = l->first;
    for (int i=0; i<idx; i++) {
        if (el==NULL) return NULL;
        el = el->next;
    }
    return el;
}

const arglist_el *arglist_get_name(arglist *l, const char *name) {
    for(arglist_el *el = l->first; el!=NULL; el=el->next) { 
        if (strcmp(name, el->name)==0) return el;
    }
    return NULL;
}

void arglist_free(arglist *l) {
    arglist_el *el = l->first;
    while (el != NULL) {
        arglist_el *free_el = el;
        el = el->next;
        free(free_el->name);
        free(free_el->val);
        free(free_el);
    }
    free(l);
}

