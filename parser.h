/**
 * iclc - Interactive Command Line Calculator
 *
 * parser.h
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

#ifndef _PARSER_H_
#define _PARSER_H_

#include "lexer.h"
#include "ast.h"

struct _PARSE_ERROR {
    int position;
    char *message;

    struct _PARSE_ERROR *_next;
};

// Parserkontext
struct _PARSE_CONTEXT {
    ast_node *ast_root;
    lex_context *lex_ctx;
    int token;
    struct _PARSE_ERROR *error;
};

typedef struct _PARSE_ERROR parse_error;
typedef struct _PARSE_CONTEXT parse_context;

parse_context *start_parse(const char *input);
void end_parse(parse_context *ctx);

int parse(parse_context *ctx);

ast_node *parse_expr(parse_context *ctx);
ast_node *parse_primary(parse_context *ctx);

#endif // _PARSER_H_
