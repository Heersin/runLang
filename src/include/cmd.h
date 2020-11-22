#ifndef _CMD_H
#define _CMD_H

#include "mpc.h"
#include "lang.h"
#include <stdio.h>
#include <string.h>

enum { CMD_ERR, CMD_NO, CMD_RES }

// cmd syntax :
// [cmd_name] [arg1] [arg2] ...

// support result struct

// Actually, no need to put struct definition here
// because no function will call it except those in cmd.c
// but easy to read if put here
struct CmdValStruct
{
    int type;
    char *result;

    int argc;
    char **argv;
};

typedef struct CmdValStruct *cmdval;

LangParser init_cmd(void);
void clean_cmd(LangParser parser);
cmdval cmd_read(mpc_ast_t *t);
cmdval cmd_eval(cmdval v);
void cmd_print(cmdval v);
void cmd_del(cmdval v);
#endif