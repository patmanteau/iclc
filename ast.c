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
 * Copyright 2012 Patrick Haas (@p7haas)
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
    case EXPR_FUNC: {
        expr_func_data *func_data = (expr_func_data*)tree->data;
        if (strcmp(func_data->name, "sin") == 0) return sin(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "cos") == 0) return cos(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "tan") == 0) return tan(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "acos") == 0) return acos(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "asin") == 0) return asin(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "atan") == 0) return atan(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "cosh") == 0) return cosh(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "sinh") == 0) return sinh(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "tanh") == 0) return tanh(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "log") == 0) return log(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "log10") == 0) return log10(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "ceil") == 0) return ceil(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "fabs") == 0) return fabs(calc_expr_ast(func_data->rhs));
        else if (strcmp(func_data->name, "floor") == 0) return floor(calc_expr_ast(func_data->rhs));
        else fprintf(stderr, "Unknown function: %s\n", func_data->name); return -1;
    }
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
        default: fprintf(stderr, "Operator not implemented\n"); return -1;
        }
    }
    case EXPR_UNOP: {
        expr_unop_data *unop_data = (expr_unop_data*)tree->data;
        switch (unop_data->op) {
        case OP_POS:
            return calc_expr_ast(unop_data->rhs);
        case OP_NEG:
            return -1.0 * calc_expr_ast(unop_data->rhs);
        default: fprintf(stderr, "Unknown unary operator: %c\n", unop_data->op); return -1;
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
    case EXPR_FUNC: {
        expr_func_data *func_data = (expr_func_data*)tree->data;
        printf("%s(", func_data->name);
        print_expr_ast(func_data->rhs);
        printf(")");
        break;
    }
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
    case EXPR_FUNC: {
        expr_func_data *func_data = (expr_func_data*)tree->data;
        printf("%s:\n", func_data->name);
        dump_tree(func_data->rhs, depth+1);
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

void ast_free_node(ast_node *node) {
    if (node != NULL) {
        switch (node->type) {
            // Funktionen: rhs-Subtree rekursiv freigeben
        case EXPR_FUNC: ast_free_node(((expr_func_data*)node->data)->rhs); free(((expr_func_data*)node->data)->name); break;
            // Binäre Ausdrücke: lhs- und rhs-Subtree rekursiv freigeben
        case EXPR_BINOP: ast_free_node(((expr_binop_data*)node->data)->lhs); ast_free_node(((expr_binop_data*)node->data)->rhs); break;
            // Unäre Ausdrücke: rhs-Subtree rekursiv freigeben
        case EXPR_UNOP: ast_free_node(((expr_unop_data*)node->data)->rhs); break;
            // No-ops: Folgeausdruck freigeben
        case EXPR_NOP: ast_free_node(((expr_nop_data*)node->data)->next); break;
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

