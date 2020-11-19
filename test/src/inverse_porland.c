/*
Aim to build a command interpreter
A prototype now
*/
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "mpc.h"

// >>>>>>>>>>>> Main <<<<<<<<<<<<<<<<<<<<
int main(int argc, char **argv)
{

    // create lang 
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                     \
        number   : /-?[0-9]+.*[0-9]*/ ;                             \
        operator : '+' | '-' | '*' | '/' | '%';                  \
        expr     : <number> | '(' <operator> <expr>+ ')' ;  \
        lispy    : /^/ <operator> <expr>+ /$/ ;             \
        ",
        Number, Operator, Expr, Lispy);
    

    // prompt, can be read from config in the future
    const char *prompt = "[console]> ";
    char *cmd_buffer = NULL;

    // display the welcome info
    printf("Welcome Hereeee, Ctrl-C to quit\n");

    // A while loop to recv input
    while((cmd_buffer = readline(prompt)) != NULL)
    {
        if (strlen(cmd_buffer) > 0)
            add_history(cmd_buffer);
        
        mpc_result_t r;
        if (mpc_parse("<stdin>", cmd_buffer, Lispy, &r)){
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        }
        else{
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        // free
        free(cmd_buffer);
    }

    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}