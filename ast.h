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
 * Copyright 2012 Patrick Haas (@p7haas)
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
#include "arglist.h"

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
    OP_ASS = '=',
};

// Knotentypen des Syntaxbaums
enum parser_ast_node_t {
    EXPR_NUM = -1,
    EXPR_BINOP = -2,
    EXPR_UNOP = -3,
    EXPR_FUNC = -4,
    EXPR_VAR  = -5,

    EXPR_NOP = -255,
};

// "Abstrakter" Knotentyp des Syntaxbaums
struct _AST_NODE {
    int type;
    void *data;
};

// Datenstruktur für numerische Literalausdrücke
struct _EXPR_NUM_DATA {
    LONG_DOUBLE val;
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

// Datenstruktur für Funktionsaufrufe
struct _EXPR_FUNC_DATA {
    char *name;
    arglist *args;
};

// Datenstruktur für Funktionsdefinitionen
struct _FUNC_DEF_DATA {
    char *name;
    arglist *args;    
    struct _AST_NODE *rhs;
};

// Datenstruktur für Variablen
struct _EXPR_VAR_DATA {
    char *name;
};

// Datenstruktur für No-ops
struct _EXPR_NOP_DATA {
    struct _AST_NODE *next;
};


typedef struct _AST_NODE ast_node;
typedef struct _EXPR_NUM_DATA expr_num_data;
typedef struct _EXPR_UNOP_DATA expr_unop_data;
typedef struct _EXPR_BINOP_DATA expr_binop_data;
typedef struct _EXPR_FUNC_DATA expr_func_data;
typedef struct _EXPR_VAR_DATA expr_var_data;
typedef struct _EXPR_NOP_DATA expr_nop_data;
typedef struct _FUNC_DEF_DATA func_def_data;

// Baum auf stdout ausgeben
void dump_tree(ast_node *tree, int depth);

// AST-Knoten und alle Subknoten freigeben
void ast_free_node(ast_node *node);

#endif // _AST_H_
