/**
 * iclc - Interactive Command Line Calculator
 *
 * snippets.h
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

#ifndef _SNIPPETS_H_
#define _SNIPPETS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void *safe_malloc(size_t num);
void *safe_calloc(int num, size_t size);

char *stringalloc(int len);
char *stringcopy(const char *src);

#endif // _SNIPPETS_H_
