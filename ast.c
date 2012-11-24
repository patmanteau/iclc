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

void dump_tree(ast_node *tree, int depth) {
    for (int i = 0; i<depth; i++) printf("  ");
    switch (tree->type) {
    case EXPR_NUM: {
        expr_num_data *num_data = (expr_num_data*)tree->data;
        printf("%Lg\n", num_data->val);
        break;
    }
    case EXPR_FUNC: {
        expr_func_data *func_data = (expr_func_data*)tree->data;
        printf("%s(", func_data->name);
        for (ast_node_ptr *arg = list_first(ast_node_ptr, func_data->args);
             arg;
             arg = list_next(ast_node_ptr, arg)
             ) {
            dump_tree(*arg, depth+1);
            if (arg != list_last(ast_node_ptr, func_data->args)) {
                printf(", ");
            }
        }
        printf(")\n");
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
            // Funktionen: args-Subtrees rekursiv freigeben
        case EXPR_FUNC: {
            list_finalize(((expr_func_data*)node->data)->args);
            free(((expr_func_data*)node->data)->name); break;
        }
            // Binäre Ausdrücke: lhs- und rhs-Subtree rekursiv freigeben
        case EXPR_BINOP: ast_free_node(((expr_binop_data*)node->data)->lhs); ast_free_node(((expr_binop_data*)node->data)->rhs); break;
            // Unäre Ausdrücke: rhs-Subtree rekursiv freigeben
        case EXPR_UNOP: ast_free_node(((expr_unop_data*)node->data)->rhs); break;
            // No-ops: Folgeausdruck freigeben
        case EXPR_NOP: ast_free_node(((expr_nop_data*)node->data)->next); break;
            // Variable: Namen freigeben
        case EXPR_VAR: free(((expr_var_data*)node->data)->name); break;
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

void ast_free_node_ptr(void *data) {
    ast_free_node(*((ast_node_ptr*)data));
}
