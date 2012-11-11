/**
 * iclc - Interactive Command Line Calculator
 *
 * lexer.c
 *
 * iclc is a simple, interactive, command line calculator. It
 * employs and thereby demonstrates basic parsing technique. 
 *
 * Beware, it leaks like a strainer and probably has more bugs
 * than Starship Troopers.
 *
 *
 * Copyright 2012 Patrick Haas (@patmanteau)
 *
 * Released under the MIT and GPL licenses.
 *
 * ------------------------------------------------
 *  author:  Patrick Haas
 *  version: 1.0.1
 *  url:     https://github.com/p7haas/iclc
 *  source:  https://github.com/p7haas/iclc
 */                                                   

#include "lexer.h"

const size_t LEX_LEXEME_BUFFER_SIZE = 8192;

lex_context *lex_open(const char *input) {
    lex_context *ctx = malloc(sizeof(lex_context));
    ctx->__input = ctx->input = calloc(strlen(input), sizeof(char));
    strcpy(ctx->__input, input);
    ctx->_end_of_input = ctx->__input+strlen(input);
    return ctx;
}

void lex_close(lex_context *ctx) {
    free(ctx->__input);
    free(ctx);
}

int lex_get_token(lex_context *ctx) {
    // Status leeren
    ctx->last_token_double = 0;
    ctx->last_token_char = 0;
    // Whitespace überspringen
    while (isspace(*(ctx->input))) ctx->input++; 

    // Ziffern ([+-][0-9.]+)
    if (isdigit(*(ctx->input)) || (*(ctx->input) == '.')) {
        char *end = NULL;
        ctx->last_token_double = strtod(ctx->input, &end);
        ctx->input = end;
        return TOK_NUM;
    }

    // öffnende Klammer
    if (*(ctx->input) == '(') {
        ctx->last_token_char = *(ctx->input++);
        return TOK_PAREN_OPEN;
    }

    if (*(ctx->input) == ')') {
        ctx->last_token_char = *(ctx->input++);
        return TOK_PAREN_CLOSE;
    }

    // Ende der Eingabe
    if ((*(ctx->input) == 0) || (ctx->input >= ctx->_end_of_input)) return TOK_EOF;

    // Alle anderen Zeichen
    ctx->last_token_char = *(ctx->input++);
    return TOK_CHAR;
}
