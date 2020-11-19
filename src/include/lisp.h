#ifndef _LISP_H
#define _LISP_H

#include "mpc.h"
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };

struct LispParserStruct
{
    mpc_parser_t *Number;
    mpc_parser_t *Symbol;
    mpc_parser_t *Sexpr;
    mpc_parser_t *Expr;
    mpc_parser_t *Lang;
};

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


typedef struct LispParserStruct *LispParser;
typedef struct LispVal  *lval;

LispParser init_lisp();
void clean_lisp(LispParser parser);

// Lisp parser


#endif