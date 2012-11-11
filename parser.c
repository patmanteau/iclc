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

#include "parser.h"

parse_context *start_parse(const char *input) {
    parse_context *p_ctx = malloc(sizeof(parse_context));

    // Lexer-Kontext erzeugen
    p_ctx->lex_ctx = lex_open(input);
    p_ctx->token = TOK_NOP;
    return p_ctx;
}

int parse_get_next_token(parse_context *ctx) {
    ctx->token = lex_get_token(ctx->lex_ctx);
    return ctx->token;
}

ast_node *parse_emit_error(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    return NULL;
}

ast_node *parse_number_expr(parse_context *ctx) {
    ast_node *node = malloc(sizeof(ast_node));
    node->type = EXPR_NUM;
    expr_num_data *data = malloc(sizeof(expr_num_data));
    data->val = ctx->lex_ctx->last_token_double;
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
    if (ctx->token != TOK_PAREN_CLOSE) return parse_emit_error(") expected.");

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
        int tok_precedence = parse_operator_precedence(ctx->lex_ctx->last_token_char);
        if (tok_precedence < expr_precedence) return lhs;
        
        // Operator speichern und verbrauchen
        int op = ctx->lex_ctx->last_token_char;
        parse_get_next_token(ctx);

        // Primärausdruck rechts vom Operator verarbeiten
        ast_node *rhs = parse_primary(ctx);
        if (!rhs) return NULL;

        // kommt noch ein Operator?
        int next_precedence = parse_operator_precedence(ctx->lex_ctx->last_token_char);
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
    data->op = ctx->lex_ctx->last_token_char;
    
    parse_get_next_token(ctx);
    
    data->rhs = parse_primary(ctx);

    // Fehler weiterreichen
    if (!data->rhs) return NULL;
 
    node->data = data;

    return node;    
}

ast_node *parse_primary(parse_context *ctx) {
    switch (ctx->token) {
    case TOK_CHAR: return parse_unop_expr(ctx);
    case TOK_NUM: return parse_number_expr(ctx);
    case TOK_PAREN_OPEN: return parse_paren_expr(ctx);
        //case TOK_NOP: parse_get_next_token(ctx); return parse_primary(ctx);
    default: return parse_emit_error("Not an expression.");
    }
}

void parse(parse_context *ctx) {
    parse_get_next_token(ctx);
    ctx->ast_root = parse_expr(ctx);
}

void free_node(ast_node *node) {
    if (node != NULL) {
        switch (node->type) {
            // Binäre Ausdrücke: lhs- und rhs-Subtree rekursiv freigeben
        case EXPR_BINOP: free_node(((expr_binop_data*)node->data)->lhs); free_node(((expr_binop_data*)node->data)->rhs); break;
            // Unäre Ausdrücke: rhs-Subtree rekursiv freigeben
        case EXPR_UNOP: free_node(((expr_unop_data*)node->data)->rhs); break;
            // No-ops: Folgeausdruck freigeben
        case EXPR_NOP: free_node(((expr_nop_data*)node->data)->next); break;
            // Numerische Literale: nichts freizugeben
        case EXPR_NUM: break;
            // Uh-oh...
        default: printf("ARRRRR, UNKNOWN NODE TYPE!\n\n"); break;
        }
        // Datenstruktur freigeben
        free(node->data);
        // Abstrakten Knoten freigeben
        free(node);
    }
}

void end_parse(parse_context *ctx) {
    if (ctx != NULL) {
        // Aufräumen
        free_node(ctx->ast_root);
        lex_close(ctx->lex_ctx);
        free(ctx);
    }
}
