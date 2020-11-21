#ifndef _LISP_H
#define _LISP_H

#include "mpc.h"
#include "lang.h"
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };

struct LispVal
{
    int type;
    long num;

    /*Error and symbol types have some string*/
    char *err;
    char *sym;

    /*can contain lval, count is the size of consequenced lval*/
    int count;
    struct LispVal** cell;
};
typedef struct LispVal  *lval;

LangParser init_lisp(void);
void clean_lisp(LangParser parser);

// Lisp parser
lval lval_eval(lval v);
lval lval_eval_sexpr(lval v);
lval builtin_op(lval a, char *op);

lval lval_add(lval v, lval x);
void lval_del(lval v);
lval lval_pop(lval v, int i);
lval lval_take(lval v, int i);
void _lval_print(lval v);
void lval_println(lval v);
lval lval_read(mpc_ast_t *t);

lval _lval_read_num(long x);
lval lval_read_num(mpc_ast_t *t);
lval lval_read_sym(char *sym);
lval lval_read_sexpr(void);
lval lval_read_err(char *msg);

#endif