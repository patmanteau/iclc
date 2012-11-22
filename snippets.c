/**
 * iclc - Interactive Command Line Calculator
 *
 * snippets.c
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

#include "snippets.h"

void *safe_malloc(size_t num) {
    void *p = malloc(num);
    if (p == NULL) {
        perror(NULL);
        exit(-1);
    }
    return p;
}

void *safe_calloc(int num, size_t size) {
    void *p = calloc(num, size);
    if (p == NULL) {
        perror(NULL);
        exit(-1);
    }
    return p;
}

char *stringalloc(int len) {
    return safe_calloc(len+1, sizeof(char));
}

char *stringcopy(const char *src) {
    char *str = stringalloc(strlen(src));
    return strcpy(str, src);
}
    
