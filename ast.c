/**
 * iclc - Interactive Command Line Calculator
 *
 * ast.c
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


#include "ast.h"

#include <math.h>

double calc_expr_ast(ast_node *tree) {
    if (!tree) return 0;

    switch (tree->type) {
    case EXPR_NUM: {
        expr_num_data *num_data = (expr_num_data*)tree->data;
        return num_data->val;
        break;
    }
    case EXPR_BINOP: {
        expr_binop_data *binop_data = (expr_binop_data*)tree->data;
        switch (binop_data->op) {
        case OP_ADD:
            return calc_expr_ast(binop_data->lhs) + calc_expr_ast(binop_data->rhs);
        case OP_SUB:
            return calc_expr_ast(binop_data->lhs) - calc_expr_ast(binop_data->rhs);
        case OP_MUL:
            return calc_expr_ast(binop_data->lhs) * calc_expr_ast(binop_data->rhs);
        case OP_DIV:
            return calc_expr_ast(binop_data->lhs) / calc_expr_ast(binop_data->rhs);
        case OP_MOD: {
            // Knuthsche "floored division"
            double a = calc_expr_ast(binop_data->lhs);
            double n = calc_expr_ast(binop_data->rhs);
            return a-n*floor(a/n);
        }
        case OP_EXP:
            return pow(calc_expr_ast(binop_data->lhs), calc_expr_ast(binop_data->rhs));
        default: fprintf(stderr, "Operator not implemented\n"); break;
        }
    }
    case EXPR_UNOP: {
        expr_unop_data *unop_data = (expr_unop_data*)tree->data;
        switch (unop_data->op) {
        case OP_POS:
            return calc_expr_ast(unop_data->rhs);
        case OP_NEG:
            return -1.0 * calc_expr_ast(unop_data->rhs);
        default: fprintf(stderr, "Illegal unary operator\n"); break;
        }
    }
    case EXPR_NOP: {
        expr_nop_data *nop_data = (expr_nop_data*)tree->data;
        return calc_expr_ast(nop_data->next);
    }
    default:
        printf("Unknown expression\n");
        return -1;
    }
}

void print_expr_ast(ast_node *tree) {
    if (!tree) return;
    switch (tree->type) {
    case EXPR_NUM: {
        expr_num_data *num_data = (expr_num_data*)tree->data;
        printf("%g", num_data->val);
        break;
    }
    case EXPR_BINOP: {
        expr_binop_data *binop_data = (expr_binop_data*)tree->data;
        printf("(");
        print_expr_ast(binop_data->lhs);
        switch (binop_data->op) {
        case OP_ADD:
            printf("+"); break;
        case OP_SUB:
            printf("-"); break;
        case OP_MUL:
            printf("*"); break;
        case OP_DIV:
            printf("/"); break;
        case OP_MOD:
            printf("%%"); break;
        case OP_EXP:
            printf("^"); break;
        default:
            printf("?"); break;
        }
        print_expr_ast(binop_data->rhs);
        printf(")");
        break;
    }
    case EXPR_UNOP: {
        expr_unop_data *unop_data = (expr_unop_data*)tree->data;
        printf("%c", unop_data->op);
        print_expr_ast(unop_data->rhs);
        break;
    }
    case EXPR_NOP: {
        expr_nop_data *nop_data = (expr_nop_data*)tree->data;
        print_expr_ast(nop_data->next);
        break;
    }
    default:
        printf("!\n");
        break;
    }   
}

void dump_tree(ast_node *tree, int depth) {
    for (int i = 0; i<depth; i++) printf("  ");
    switch (tree->type) {
    case EXPR_NUM: {
        expr_num_data *num_data = (expr_num_data*)tree->data;
        printf("%g\n", num_data->val);
        break;
    }
    case EXPR_BINOP: {
        expr_binop_data *binop_data = (expr_binop_data*)tree->data;
        printf("%c:\n", binop_data->op);
        dump_tree(binop_data->lhs, depth+1);
        dump_tree(binop_data->rhs, depth+1);
        break;
    }
    case EXPR_UNOP: {
        expr_unop_data *unop_data = (expr_unop_data*)tree->data;
        printf("%c:\n", unop_data->op);
        dump_tree(unop_data->rhs, depth+1);
        break;
    }
    case EXPR_NOP: {
        expr_nop_data *nop_data = (expr_nop_data*)tree->data;
        printf("NOP\n");
        dump_tree(nop_data->next, depth);
        break;
    }
    default:
        printf("Unknown\n");
        break;
    }   
}
