/**
 * iclc - Interactive Command Line Calculator
 *
 * eval.h
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

#ifndef _EVAL_H_
#define _EVAL_H_

#include "ast.h"

struct _EVAL_ERROR {
    char *message;

    struct _EVAL_ERROR *__next;
};

struct _EVAL_CONTEXT {
    bool success;
    double result;

    struct _EVAL_ERROR *error;
};

typedef struct _EVAL_ERROR eval_error;
typedef struct _EVAL_CONTEXT eval_context;

// Ausdruck auswerten und Ergebnis zurückliefern.
// Rückgabewert sollte vom Aufrufer mit free()
// freigegeben werden.
eval_context *eval(ast_node *tree);

double eval_expr(eval_context *ctx, ast_node *tree);

void evalprintf(ast_node *tree);
// Auswertungskontext freigeben
void end_eval(eval_context *ctx);

#endif // _EVAL_H_
