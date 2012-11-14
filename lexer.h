/**
 * iclc - Interactive Command Line Calculator
 *
 * lexer.h
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

#ifndef _LEXER_H_
#define _LEXER_H_

#include "main.h"

enum lex_token_t {
    TOK_EOF = -1,
    TOK_NUM = -2,
    TOK_CHAR = -3,
    TOK_OPER = -4,
    TOK_IDENT = -5,

    TOK_PAREN_OPEN = -101,
    TOK_PAREN_CLOSE = -102,

    TOK_NOP = -255,
};

struct _LEX_CONTEXT {
    // pointer to first input character for free()'ing
    char *__input;
    char *_end_of_input;
    char *cursor;

    double token_double;
    char token_char;
    char *token_string;
};

typedef struct _LEX_CONTEXT lex_context;

lex_context *lex_open(const char *input);
void lex_close(lex_context *ctx);
int lex_get_offset(lex_context *ctx);
int lex_get_token(lex_context *ctx);

#endif 




