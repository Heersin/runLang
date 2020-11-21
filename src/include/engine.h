#ifndef _ENGINE_H
#define _ENGINE_H
#include "mpc.h"
#include "lang.h"
#include "lisp.h"
#include <string.h>

int handle_cmd(char *input);
void check_result(int result);

// init and clean global var
void init_context(void);
void clean_context(void);

#endif