/**
 * iclc - Interactive Command Line Calculator
 *
 * eval.c
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

#include "eval.h"
#include "store.h"
#include "snippets.h"

#include <math.h>
#include <stdarg.h>

const int MAX_PN = 255;

LONG_DOUBLE eval_emit_error(eval_context *ctx, const char *format, ...) {
    // Fehlerstring aufbauen, printf-Syntax
    va_list args;
    va_start(args, format);
    char fmtbuf[256];
    vsnprintf(fmtbuf, 256, format, args);

    // Fehlerstruktur füllen
    eval_error *e = malloc(sizeof(eval_error));
    e->__next = NULL;
    e->message = stringcopy(fmtbuf);

    // Letzten Eintrag in der Fehlerliste finden und e anhängen
    eval_error **it = &ctx->error;
    while (*it != NULL) it = &((*it)->__next);
    *it = e;
    
    // Fehlerflag setzen.
    ctx->success = false;

    return -NAN;
}

LONG_DOUBLE eval_expr_func(eval_context *ctx, store *st, ast_node *tree) {
    expr_func_data *func_data = (expr_func_data*)tree->data;
    if (strcmp(func_data->name, "sin") == 0) return sinl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "cos") == 0) return cosl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "tan") == 0) return tanl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "acos") == 0) return acosl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "asin") == 0) return asinl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "atan") == 0) return atanl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "cosh") == 0) return coshl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "sinh") == 0) return sinhl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "tanh") == 0) return tanhl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "log") == 0) return logl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "log10") == 0) return log10l(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "ceil") == 0) return ceill(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "fabs") == 0) return fabsl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "floor") == 0) return floorl(eval_expr(ctx, st, func_data->rhs));
    else if (strcmp(func_data->name, "sqrt") == 0) return sqrtl(eval_expr(ctx, st, func_data->rhs));
    else return eval_emit_error(ctx, "Unknown function %s.", func_data->name);
}

LONG_DOUBLE eval_expr_var(eval_context *ctx, store *st, ast_node *tree) {
    expr_var_data *var_data = (expr_var_data*)tree->data;
    if (store_has_var(st, var_data->name)) return store_get_var(st, var_data->name);
    else return eval_emit_error(ctx, "Undeclared variable %s.", var_data->name);
}

LONG_DOUBLE eval_expr_num(eval_context *ctx, store *st, ast_node *tree) {
    expr_num_data *num_data = (expr_num_data*)tree->data;
    return num_data->val;
}

LONG_DOUBLE eval_expr_binop(eval_context *ctx, store *st, ast_node *tree) {
    expr_binop_data *binop_data = (expr_binop_data*)tree->data;
    switch (binop_data->op) {
    case OP_ADD:
        return eval_expr(ctx, st, binop_data->lhs) + eval_expr(ctx, st, binop_data->rhs);
    case OP_SUB:
        return eval_expr(ctx, st, binop_data->lhs) - eval_expr(ctx, st, binop_data->rhs);
    case OP_MUL:
        return eval_expr(ctx, st, binop_data->lhs) * eval_expr(ctx, st, binop_data->rhs);
    case OP_DIV:
        return eval_expr(ctx, st, binop_data->lhs) / eval_expr(ctx, st, binop_data->rhs);
    case OP_MOD: {
        // Knuthsche "floored division"
        LONG_DOUBLE a = eval_expr(ctx, st, binop_data->lhs);
        LONG_DOUBLE n = eval_expr(ctx, st, binop_data->rhs);
        return a-n*floor(a/n);
    }
    case OP_EXP:
        return pow(eval_expr(ctx, st, binop_data->lhs), eval_expr(ctx, st, binop_data->rhs));
    case OP_ASS: {
        if (binop_data->lhs->type != EXPR_VAR) return eval_emit_error(ctx, "Cannot assign to a non-variable.");
        else {
            expr_var_data *var_data = (expr_var_data*)binop_data->lhs->data;
            LONG_DOUBLE rhs_res = eval_expr(ctx, st, binop_data->rhs);
            store_var(st, var_data->name, rhs_res);
            return rhs_res;
        }
    }
    default: return eval_emit_error(ctx, "Operator not implemented.");
    }
}

LONG_DOUBLE eval_expr_unop(eval_context *ctx, store* st, ast_node *tree) {
    expr_unop_data *unop_data = (expr_unop_data*)tree->data;
    switch (unop_data->op) {
    case OP_POS:
        return eval_expr(ctx, st, unop_data->rhs);
    case OP_NEG:
        return -1.0 * eval_expr(ctx, st, unop_data->rhs);
    default: return eval_emit_error(ctx, "Unknown unary operator %c.", unop_data->op);
    }
}

LONG_DOUBLE eval_expr_nop(eval_context *ctx, store *st, ast_node *tree) {
    expr_nop_data *nop_data = (expr_nop_data*)tree->data;
    return eval_expr(ctx, st, nop_data->next);
}


LONG_DOUBLE eval_expr(eval_context *ctx, store *st, ast_node *tree) {
    if (!tree) return eval_emit_error(ctx, "Evaluation error: node is NULL.");

    switch (tree->type) {
    case EXPR_FUNC: return eval_expr_func(ctx, st, tree);
    case EXPR_VAR: return eval_expr_var(ctx, st, tree);
    case EXPR_NUM: return eval_expr_num(ctx, st, tree);
    case EXPR_BINOP: return eval_expr_binop(ctx, st, tree);
    case EXPR_UNOP: return eval_expr_unop(ctx, st, tree);
    case EXPR_NOP: return eval_expr_nop(ctx, st, tree);
    default:return eval_emit_error(ctx, "Unknown expression.");
    }
}

eval_context *eval(ast_node *tree) {
    eval_context *ctx = malloc(sizeof(eval_context));
    ctx->success = true;
    ctx->error = NULL;

    store *st = store_get_global();

    ctx->result = eval_expr(ctx, st, tree);

    return ctx;
}

void free_errors(eval_error *e) {
    while (e != NULL) {
        eval_error *free_e = e;
        e = e->__next;
        free(free_e->message);
        free(free_e);
    }
}

void end_eval(eval_context *ctx) {
    if (ctx != NULL) {
        // Aufräumen
        free_errors(ctx->error);
        free(ctx);
    }
}

void evalprintf(ast_node *tree) {
    if (!tree) return;
    
    switch (tree->type) {
    case EXPR_FUNC: {
        expr_func_data *func_data = (expr_func_data*)tree->data;
        printf("%s(", func_data->name);
        evalprintf(func_data->rhs);
        printf(")");
        break;
    }
    case EXPR_VAR: {
        expr_var_data *var_data = (expr_var_data*)tree->data;
        printf("%s", var_data->name);
        break;
    }
    case EXPR_NUM: {
        expr_num_data *num_data = (expr_num_data*)tree->data;
        printf("%Lg", num_data->val);
        break;
    }
    case EXPR_BINOP: {
        expr_binop_data *binop_data = (expr_binop_data*)tree->data;
        printf("(");
        evalprintf(binop_data->lhs);
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
        case OP_ASS:
            printf("="); break;
        default:
            printf("?"); break;
        }
        evalprintf(binop_data->rhs);
        printf(")");
        break;
    }
    case EXPR_UNOP: {
        expr_unop_data *unop_data = (expr_unop_data*)tree->data;
        printf("%c", unop_data->op);
        evalprintf(unop_data->rhs);
        break;
    }
    case EXPR_NOP: {
        expr_nop_data *nop_data = (expr_nop_data*)tree->data;
        evalprintf(nop_data->next);
        break;
    }
    default:
        printf("!\n");
        break;
    }

    return;
}
