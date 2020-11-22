#include "mpc.h"
#include <readline/readline.h>
#include <readline/history.h>
int main(int argc, char** argv) {

  /* Create Some Parsers */
  mpc_parser_t *Name = mpc_new("cmd_name");
  mpc_parser_t *Arg = mpc_new("arg");

  mpc_parser_t *Cmd = mpc_new("cmd");

  mpca_lang(MPCA_LANG_DEFAULT,
      "                                          \
      arg : /[0-9a-zAZ_]+/ ;                    \
      cmd_name : \"help\" | \"version\" | \"info\" ; \
      cmd  : /^/ <cmd_name> <arg>* /$/ ;       \
      ",
      Arg, Name, Cmd);


  while (1) {

    char* input = readline("cmd> ");
    add_history(input);

    /* Attempt to parse the user input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Cmd, &r)) {
      /* On success print and delete the AST */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise print and delete the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  /* Undefine and delete our parsers */
  mpc_cleanup(3, Name, Arg, Cmd);

  return 0;
}
