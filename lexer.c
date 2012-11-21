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
 * Copyright 2012 Patrick Haas (@p7haas)
 *
 * Released under the MIT and GPL licenses.
 *
 * ------------------------------------------------
 *  author:  Patrick Haas
 *  url:     https://github.com/p7haas/iclc
 *  source:  https://github.com/p7haas/iclc
 */                                                   

#include "lexer.h"

#include "snippets.h"

const size_t LEX_LEXEME_BUFFER_SIZE = 8192;

lex_context *lex_open(const char *input) {
    lex_context *ctx = malloc(sizeof(lex_context));
    ctx->__input = ctx->cursor = stringcopy(input);
    ctx->_end_of_input = ctx->__input+strlen(input);
    ctx->token_string = NULL;
    return ctx;
}

void lex_close(lex_context *ctx) {
    free(ctx->token_string);
    free(ctx->__input);
    free(ctx);
}

int lex_get_offset(lex_context *ctx) {
    return ctx->cursor - ctx->__input;
}

int lex_get_token(lex_context *ctx) {
    // Status leeren
    ctx->token_double = 0;
    ctx->token_char = 0;
    free(ctx->token_string);
    ctx->token_string = NULL;

    // Whitespace überspringen
    while (isspace(*(ctx->cursor))) ctx->cursor++; 

    // Ziffern ([0-9.]+)
    if (isdigit(*(ctx->cursor)) || (*(ctx->cursor) == '.')) {
        char *end = NULL;
        ctx->token_double = STRTOLD(ctx->cursor, &end);
        ctx->token_char = *(end-1);
        ctx->cursor = end;
        return TOK_NUM;
    }

    // öffnende Klammer
    if (*(ctx->cursor) == '(') {
        ctx->token_char = *(ctx->cursor++);
        return TOK_PAREN_OPEN;
    }

    if (*(ctx->cursor) == ')') {
        ctx->token_char = *(ctx->cursor++);
        return TOK_PAREN_CLOSE;
    }

    // Ende der Eingabe
    if ((*(ctx->cursor) == 0) || (ctx->cursor >= ctx->_end_of_input)) {
        return TOK_EOF;
    }

    // Operatoren
    if ((*(ctx->cursor) == '+') || (*(ctx->cursor) == '-') ||
        (*(ctx->cursor) == '*') || (*(ctx->cursor) == '/') ||
        (*(ctx->cursor) == '%') || (*(ctx->cursor) == '^') ||
        (*(ctx->cursor) == '=')) {
        ctx->token_char = *(ctx->cursor++);
        return TOK_OPER;
    }

    if (isalpha(*(ctx->cursor))) {
        char ident_name[LEX_LEXEME_BUFFER_SIZE];
        int i = 0;
        while (isalnum(*(ctx->cursor)) && 
               (*(ctx->cursor) != 0)   &&
               (ctx->cursor < ctx->_end_of_input) &&
               (i < LEX_LEXEME_BUFFER_SIZE-1)) {
            ident_name[i++] = *(ctx->cursor++);
        }
        ident_name[i] = 0;
        ctx->token_string = stringcopy(ident_name);
        return TOK_IDENT;
    }

    // Alle anderen Zeichen
    ctx->token_char = *(ctx->cursor++);
    return TOK_CHAR;
}
