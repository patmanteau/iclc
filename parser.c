/**
 * iclc - Interactive Command Line Calculator
 *
 * parser.c
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

#include "parser.h"

const int IDENTIFIER_BUFFER_SIZE = 8192;

parse_context *start_parse(const char *input) {
    parse_context *p_ctx = malloc(sizeof(parse_context));
    memset(p_ctx, 0, sizeof(parse_context));
    // Lexer-Kontext erzeugen
    p_ctx->lex_ctx = lex_open(input);
    p_ctx->token = TOK_NOP;
    return p_ctx;
}

int parse_get_next_token(parse_context *ctx) {
    ctx->token = lex_get_token(ctx->lex_ctx);
    return ctx->token;
}

ast_node *parse_emit_error(parse_context *ctx, int pos, const char *msg) {
    // Fehlerstruktur füllen
    parse_error *e = malloc(sizeof(parse_error));
    e->_next = NULL;
    e->message = calloc(strlen(msg), sizeof(char));
    strcpy(e->message, msg);
    e->position = pos;

    // Letzten Eintrag in der Fehlerliste finden und e anhängen
    parse_error *i = ctx->error;
    if (i == NULL) {
        // erster Fehler
        ctx->error = e;
    }
    else {
        while (i->_next != NULL) i = i->_next;
        i->_next = e;
    }
    return NULL;
}

ast_node *parse_number_expr(parse_context *ctx) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = EXPR_NUM;
    expr_num_data *data = malloc(sizeof(expr_num_data));
    data->val = ctx->lex_ctx->token_double;
    node->data = data;

    parse_get_next_token(ctx);

    return node;
}

ast_node *parse_paren_expr(parse_context *ctx) {
    // '(' verbrauchen
    parse_get_next_token(ctx);
    // Rekursiv absteigen
    ast_node *v = parse_expr(ctx);
    
    // Fehler weiterreichen
    if (!v) return NULL;
    // Fehler werfen, falls die schließende Klammer fehlt.
    if (ctx->token != TOK_PAREN_CLOSE) return parse_emit_error(ctx, lex_get_offset(ctx->lex_ctx)+1, ") expected.");

    // ')' verbrauchen
    parse_get_next_token(ctx);
    return v;
}

// Gibt einen numerischen Präzedenzwert für den Operator op
// zurück oder -1, wenn op, kein Operator ist
int parse_operator_precedence(const int op) {
    switch (op) {
    case OP_ADD: return 20;
    case OP_SUB: return 20;
    case OP_MUL: return 40;
    case OP_DIV: return 40;
    case OP_MOD: return 40;
    case OP_EXP: return 80;
        // op ist kein Operator
    default: return -1;
    }
}

// Der in lhs übergebene linkshändige Ausdruck wird zum Aufbau des binären AST-Knotens
// verwendet.
ast_node *parse_binop_rhs(parse_context *ctx, int expr_precedence, ast_node *lhs) {
    while (1) {
        // Präzedenz des Operators unter dem Cursor holen
        int tok_precedence = parse_operator_precedence(ctx->lex_ctx->token_char);
        if (tok_precedence < expr_precedence) return lhs;
        
        // Operator speichern und verbrauchen
        int op = ctx->lex_ctx->token_char;
        parse_get_next_token(ctx);

        // Primärausdruck rechts vom Operator verarbeiten
        ast_node *rhs = parse_primary(ctx);
        if (!rhs) return NULL;

        // kommt noch ein Operator?
        int next_precedence = parse_operator_precedence(ctx->lex_ctx->token_char);
        if (tok_precedence < next_precedence) {
            // Nächster Ausdruck ist präzedent, rechtshändigen Ausdruck auf die linke
            // Hand durchreichen
            rhs = parse_binop_rhs(ctx, tok_precedence+1, rhs);
            if (!rhs) return NULL;
        }

        // Fertig, LHS-Knoten unter Binop-Knoten hängen
        ast_node *new_lhs = malloc(sizeof(ast_node));
        expr_binop_data *data = malloc(sizeof(expr_binop_data));
        data->op = op;
        data->lhs = lhs;
        data->rhs = rhs;
        new_lhs->type = EXPR_BINOP;
        new_lhs->data = data;
        lhs = new_lhs;
    }
}

ast_node *parse_expr(parse_context *ctx) {
    // Ein Ausdruck ist ein primärer Ausdruck, dem eine Sequenz von
    // [Binäroperator, Primärausdruck]-Paaren folgen kann.
    //
    // Erst also der linke Ausdruck.
    ast_node *lhs = parse_primary(ctx);
    
    // Fehler weiterreichen
    if (!lhs) return NULL;

    // Dann der rechte Teil des Ausdrucks
    return parse_binop_rhs(ctx, 0, lhs);
}

ast_node *parse_unop_expr(parse_context *ctx) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = EXPR_UNOP;
    expr_unop_data *data = malloc(sizeof(expr_unop_data));
    data->op = ctx->lex_ctx->token_char;
    
    parse_get_next_token(ctx);
    
    data->rhs = parse_primary(ctx);
    // Fehler weiterreichen
    if (!data->rhs) return NULL;
    node->data = data;

    return node;    
}

ast_node *parse_func(parse_context *ctx) {
    char *name = calloc(IDENTIFIER_BUFFER_SIZE, sizeof(char)); 
    int i = 0;
    do {
        name[i++] = ctx->lex_ctx->token_char;
        parse_get_next_token(ctx);
    } while (ctx->token == TOK_CHAR);
    name[i] = 0;

    ast_node *node = malloc(sizeof(ast_node));
    node->type = EXPR_FUNC;
    expr_func_data *data = malloc(sizeof(expr_func_data));
    data->name = calloc(strlen(name), sizeof(char));
    strcpy(data->name, name);

    data->rhs = parse_primary(ctx);
    // Fehler weiterreichen
    if (!data->rhs) return NULL;
    node->data = data;

    return node;
}

ast_node *parse_primary(parse_context *ctx) {
    switch (ctx->token) {
    case TOK_CHAR: return parse_func(ctx);
    case TOK_OPER: return parse_unop_expr(ctx);
    case TOK_NUM: return parse_number_expr(ctx);
    case TOK_PAREN_OPEN: return parse_paren_expr(ctx);
        //case TOK_NOP: parse_get_next_token(ctx); return parse_primary(ctx);
    default: return parse_emit_error(ctx, lex_get_offset(ctx->lex_ctx), "Function, unary operator, number or ( expected.");
    }
}

int parse(parse_context *ctx) {
    parse_get_next_token(ctx);
    ctx->ast_root = parse_expr(ctx);

    if (ctx->error != NULL) return -1;
    else return 0;
}

void end_parse(parse_context *ctx) {
    if (ctx != NULL) {
        // Aufräumen
        ast_free_node(ctx->ast_root);
        lex_close(ctx->lex_ctx);
        parse_error *error = ctx->error;
        while (error != NULL) {
            parse_error *free_error = error;
            error = error->_next;
            free(free_error->message);
            free(free_error);
        }
        free(ctx);
    }
}
