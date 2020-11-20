#include "lang.h"

LangParser init_parser(int size)
{
    LangParser parser = NULL;

    parser = (LangParser) malloc(sizeof(struct LangParserStruct));    
    if (parser == NULL)
    {
        printf("[x]Init Parser Error\n");
        return NULL;
    }

    parser->rule = (mpc_parser_t **)malloc(sizeof(mpc_parser_t *) * size);
    if (parser->rule == NULL)
    {
        printf("[x]Init Parser Rule Error\n");
        free(parser);
        return NULL;
    }

    // init value 
    for (int i = 0; i < size; ++i)
        parser->rule[i] = NULL;
    parser->size = size;
    parser->lang = NULL;

    return parser;
}


void clean_parser(LangParser parser)
{
    if (parser == NULL)
    {
        printf("[-]NULL Parser to clean up\n");
        return;
    }
    
    if (parser->rule == NULL)
    {
        printf("[-]Warning : Broken parser detected, I will free it \n");
        free(parser);
        return;
    }

    // mpc struct free remain to upper Language Layer
    free(parser->rule);
    free(parser);
}


int map_lang(char *lang_name)
{
    if (strcmp(lang_name, "lisp") == 0)
        return LISP;
    
    if (strcmp(lang_name, "cmd") == 0)
        return CMD;

    if (strcmp(lang_name, "calc") == 0)
        return CALC;

    return -1;
}