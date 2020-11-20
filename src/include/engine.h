#ifndef _ENGINE_H
#define _ENGINE_H
#include "mpc.h"
#include "lang.h"
#include "lisp.h"
#include <string.h>

// GLOBAL LANG VAR
LangParser _LANG_PARSER;


// family
// store the pointer to the corresponding function
(LangParser (**_LANG_INITS)(void));
(void (**_LANG_CLEANS)(LangParser));

(void (**_LANG_DELS)(void *));
(void *(**_LANG_EVALS)(void *));
(void *(**_LANG_READS)(mpc_ast_t*));
(void (**_LANG_PRINTS)(void *));

int handle_cmd(char *input);
void check_result(int result);

// init and clean global var
void init_context(void);
void clean_context(void);

#endif