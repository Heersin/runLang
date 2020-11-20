#ifndef _LISP_H
#define _LISP_H

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

LangParser init_lisp();
void clean_lisp(LangParser parser);

// Lisp parser


#endif