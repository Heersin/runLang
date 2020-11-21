/*
Aim to build a command interpreter
A prototype now
*/
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "mpc.h"

// >>>>>>>>>>>>>>> MACROS AND GLOBAL VAR <<<<<<<<<<<<<<<<<<<<
/* Create Enumeration of Possible Error Types */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* Create Enumeration of Possible lval Types */
enum { LVAL_NUM, LVAL_ERR };

/* Declare New lval Struct */

struct lval_struct {
  int type;
  double num;
  int err;
};

typedef struct lval_struct lval;

lval lval_err(int x);
lval lval_num(double x);
/* Create a new number type lval */
lval lval_num(double x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

/* Create a new error type lval */
lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

// >>>>>>>>>>>>> Utilites <<<<<<<<<<<<<<<<<<<<<<<<
void lval_print(lval v);
void lval_println(lval v);

lval eval_op(lval x, char* op, lval y);
lval eval(mpc_ast_t* t);

/* Print an "lval" */
void lval_print(lval v) {
  switch (v.type) {
    /* In the case the type is a number print it */
    /* Then 'break' out of the switch. */
    case LVAL_NUM: printf("%.2f", v.num); break;

    /* In the case the type is an error */
    case LVAL_ERR:
      /* Check what type of error it is and print it */
      if (v.err == LERR_DIV_ZERO) {
        printf("Error: Division By Zero!");
      }
      if (v.err == LERR_BAD_OP)   {
        printf("Error: Invalid Operator!");
      }
      if (v.err == LERR_BAD_NUM)  {
        printf("Error: Invalid Number!");
      }
    break;
  }
}

/* Print an "lval" followed by a newline */
void lval_println(lval v) { lval_print(v); putchar('\n'); }

lval eval_op(lval x, char* op, lval y) {

  /* If either value is an error return it */
  if (x.type == LVAL_ERR) { return x; }
  if (y.type == LVAL_ERR) { return y; }

  /* Otherwise do maths on the number values */
  if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
  if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
  if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
  if (strcmp(op, "/") == 0) {
    /* If second operand is zero return error */
    return y.num == 0 
      ? lval_err(LERR_DIV_ZERO) 
      : lval_num(x.num / y.num);
  }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {

  if (strstr(t->tag, "number")) {
    /* Check if there is some error in conversion */
    errno = 0;
    double x = strtod(t->contents, NULL);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  char* op = t->children[1]->contents;  
  lval x = eval(t->children[2]);

  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;  
}

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
        number   : /-?[0-9]+\\.*[0-9]*/ ;                             \
        operator : '+' | '-' | '*' | '/' ;                  \
        expr     : <number> | '(' <operator> <expr>+ ')' ;  \
        lispy    : /^/ <operator> <expr>+ /$/ ;             \
        ",
        Number, Operator, Expr, Lispy);
    

    // prompt, can be read from config in the future
    const char *prompt = "[console]> ";
    char *cmd_buffer = NULL;

    // display the welcome info
    printf("Welcome Hereeee, Ctrl-D to quit\n");

    // A while loop to recv input
    while((cmd_buffer = readline(prompt)) != NULL)
    {
        if (strlen(cmd_buffer) > 0)
            add_history(cmd_buffer);
        
        mpc_result_t r;
        if (mpc_parse("<stdin>", cmd_buffer, Lispy, &r)){
            mpc_ast_print(r.output);
            lval result = eval(r.output);
            lval_println(result);
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