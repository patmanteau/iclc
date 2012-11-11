/**
 * iclc - Interactive Command Line Calculator
 *
 * ast.h
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
 *  url:     https://github.com/p7haas/iclc
 *  source:  https://github.com/p7haas/iclc
 */                                                   


#ifndef _AST_H_
#define _AST_H_

#include "main.h"

// Unäre Operatoren
enum unop_t {
    OP_POS = '+',
    OP_NEG = '-',
};

// Binäre Operatoren
enum binop_t {
    OP_ADD = '+',
    OP_SUB = '-',
    OP_MUL = '*',
    OP_DIV = '/',
    OP_MOD = '%',
    OP_EXP = '^',
};

// Knotentypen des Syntaxbaums
enum parser_ast_node_t {
    EXPR_NUM = -1,
    EXPR_BINOP = -2,
    EXPR_UNOP = -3,

    EXPR_NOP = -255,
};

// "Abstrakter" Knotentyp des Syntaxbaums
struct _AST_NODE {
    int type;
    void *data;
};

// Datenstruktur für numerische Literalausdrücke
struct _EXPR_NUM_DATA {
    double val;
};

// Datenstruktur für unäre Ausdrücke
struct _EXPR_UNOP_DATA {
    int op;
    struct _AST_NODE *rhs;
};

// Datenstruktur für binäre Ausdrücke
struct _EXPR_BINOP_DATA {
    int op;
    struct _AST_NODE *lhs;
    struct _AST_NODE *rhs;
};

// Datenstruktur für No-ops
struct _EXPR_NOP_DATA {
    struct _AST_NODE *next;
};


typedef struct _AST_NODE ast_node;
typedef struct _EXPR_NUM_DATA expr_num_data;
typedef struct _EXPR_UNOP_DATA expr_unop_data;
typedef struct _EXPR_BINOP_DATA expr_binop_data;
typedef struct _EXPR_NOP_DATA expr_nop_data;

double calc_expr_ast(ast_node *tree);
void print_expr_ast(ast_node *tree);
void dump_tree(ast_node *tree, int depth);

#endif // _AST_H_
