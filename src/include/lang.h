#ifndef _H_LANG
#define _H_LANG

#include "mpc.h"
#include <string.h>

#define CMD 0
#define LISP 1
#define CALC 2

struct LangParserStruct
{
    mpc_parser_t **rule;
    mpc_parser_t *lang;
    int size; // size of lang rule
};

typedef LangParserStruct *LangParser;

LangParser init_parser(int size);
void clean_parser(LangParser parser);
int map_lang(char *lang_name);

#endif