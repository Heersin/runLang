#include "engine.h"

int handle_input(char *input)
{
    char *cmd;
    int lang_type;

    // lang family
    LangParser (*_lang_init)(void);
    void (*_lang_clean)(LangParser);

    void (*_func_del)(void *);
    void *(*_func_eval)(void *);
    void *(*_func_read)(mpc_ast_t*);
    void (*_func_println)(void *);
    void *_val_x;

    // later move it as a cmd language?
    if ((cmd = strchr(input, "#")) != NULL)
    {
        cmd += 1;
        lang_type = map_lang(cmd);

        if(lang_type == NOLANG)
        {
            printf("No such a lang\n");
            return 0;
        }

        // clean old parser
        _lang_clean(_LANG_PARSER);

        // replace func family
        // maybe we can create it as a context
        // not the current GLOBAL VAR
        _lang_init = _LANG_INITS[lang_type];
        _lang_clean = _LANG_CLEANS[lang_type];
        _func_del = _LANG_DELS[lang_type];
        _func_eval = _LANG_EVALS[lang_type];
        _func_read = _LANG_READS[lang_type];
        _func_println = _LANG_PRINTS[lang_type];

        // create new parser
        _LANG_PARSER = _lang_init();
    }

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, _LANG_PARSER->lang, &r))
    {
        _val_x = (void *)_func_eval(_func_read(r.output));
        _func_println(_val_x);
        _func_del(_val_x);
        mpc_ast_delete(r.output); 
    }
    else
    {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }

    return 1;
}

void check_result(int result)
{
    if (result == 0)
        fprintf(stderr, "x");
};

void init_context(void)
{
    _LANG_INITS = (LangParser (**)(void))malloc(sizeof(void *) * MAX_LANG);
    _LANG_CLEANS = (void (**)(LangParser))malloc(sizeof(void *) * MAX_LANG);

    _LANG_DELS = (void (**)(void *))malloc(sizeof(void *) * MAX_LANG);
    _LANG_EVALS = (void *(**)(void *))malloc(sizeof(void *) * MAX_LANG);
    _LANG_READS = (void *(**)(mpc_ast_t*))malloc(sizeof(void *) * MAX_LANG);
    _LANG_PRINTS = (void (**)(void *))malloc(sizeof(void *) * MAX_LANG);

    // set context
    // init LISP
    _LANG_INITS[LISP] = &init_lisp;
    _LANG_CLEANS[LISP] = &clean_lisp;
    _LANG_DELS[LISP] = &lval_del;
    _LANG_EVALS[LISP] = &lval_eval;
    _LANG_READS[LISP] = &lval_read;
    _LANG_PRINTS[LISP] = &lval_println;

}

void clean_context(void)
{
    free(_LANG_CLEANS);
    free(_LANG_INITS);

    free(_LANG_DELS);
    free(_LANG_EVALS);
    free(_LANG_PRINTS);
    free(_LANG_READS);
}