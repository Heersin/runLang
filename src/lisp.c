#include "lisp.h"

LangParser init_lisp()
{
    LangParser lisp_parser;
    lisp_parser = init_parser(4);
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Symbol = mpc_new("symbol");
    mpc_parser_t *Sexpr = mpc_new("sexpr");
    mpc_parser_t *Expr = mpc_new("expr");

    mpc_parser_t *Lang = mpc_new("lispy");

    // define rule of this language
    mpca_lang(MPCA_LANG_DEFAULT,
        "                                          \
        number : /-?[0-9]+/ ;                    \
        symbol : '+' | '-' | '*' | '/' ;         \
        sexpr  : '(' <expr>* ')' ;               \
        expr   : <number> | <symbol> | <sexpr> ; \
        lispy  : /^/ <expr>* /$/ ;               \
        ",
        Number, Symbol, Sexpr, Expr, Lang);

    // prepare language struct
    lisp_parser->lang = Lang;
    lisp_parser->rule[0] = Number;
    lisp_parser->rule[1] = Symbol;
    lisp_parser->rule[2] = Sexpr;
    lisp_parser->rule[3] = Expr;

    return lisp_parser;
}

void clean_lisp(LangParser parser)
{
    mpc_cleanup(5, 
        parser->Lang, 
        parser->rule[0], 
        parser->rule[1], 
        parser->rule[2], 
        parser->rule[3]
    );
    clean_parser(parser);
}

// >>>>>>>>>>>>>>>>>>>> Functionality <<<<<<<<<<<<<<<<<<<

// ******************* Evaluation *****************
// Support Symbol expression
lval lval_eval(lval v)
{
    if(v->type == LVAL_SEXPR)
        return lval_eval_sexpr(v);
    return v;
}

lval lval_eval_sexpr(lval v)
{
    // evaluate children
    for (int i = 0; i < v->count; ++i)
        v->cell[i] = lval_eval(v->cell[i]);
    
    // error ?
    for (int i = 0; i < v->count; ++i)
        if(v->cell[i]->type == LVAL_ERR)
            return lval_take(v, i);
    
    // empty expression just return it self
    if (v->count == 0)
        return v;
    
    // single one
    if (v->count == 1)
        return lval_take(v, 0);
    
    // Constrain -- First element mast be symbol
    lval first_element = lval_pop(v, 0);
    if(first_element->type != LVAL_SYM)
    {
        lval_del(first_element);
        lval_del(v);
        return lval_err("Symbol Expresstion (S-expr) Should Start with a symbol\n");
    }

    // operator
    lval result = builtin_op(v, first_element->sym);
    lval_del(first_element);
    return result;
}

lval builtin_op(lval a, char *op)
{
    // all arguments should be numbers !!
    for (int i = 0; i < a->count; ++i)
        if(a->cell[i]->type != LVAL_NUM){
            lval_del(a);
            return lval_err("Non-number Element !!\n");
        }

    // pop the first
    lval x = lval_pop(a, 0);

    // only a sub 
    if ((strcmp(op, "-") == 0) && a->count == 0)
        x->num = -x->num;

    // while there are still elements remaining
    while (a->count > 0)
    {
        // keep poping
        lval y = lval_pop(a, 0);

        // perform algorithm
        if (strcmp(op, "+") == 0) { x->num += y->num; }
        if (strcmp(op, "-") == 0) { x->num -= y->num; }
        if (strcmp(op, "*") == 0) { x->num *= y->num; }
        if (strcmp(op, "/") == 0) {
            if (y->num == 0){
                // Oops error
                lval_del(x);
                lval_del(y);
                x = lval_err("Zero Division !\n");
                break;
            }
            x->num /= y->num;
        }

        lval_del(y);
    }

    lval_del(a);
    return x;
}



// ******************** AST Operation *************************
lval lval_add(lval v, lval x)
{
    v->count++;
    v->cell = realloc(v->cell, v->count * sizeof(lval));
    v->cell[v->count - 1] = x;
}

void lval_del(lval v)
{
    switch (v->type)
    {
        case LVAL_NUM:
            break;
        
        case LVAL_ERR:
            free(v->err);
            break;
        
        case LVAL_SYM:
            free(v->sym);
            break;
        
        case LVAL_SEXPR:
            for (int i = 0; i < v->count; ++i)
                lval_del(v->cell[i]);
            free(v->cell);
            break;
        
        default:
            printf("Unknown Happend!!\n");
            exit(-1);
    }
    free(v);
}

// get one element and remove it;
lval lval_pop(lval v, int i)
{
    // get the index i element
    lval x = v->cell[i];

    // shift memory after the item
    memmove(&v->cell[i], &v->cell[i+1], sizeof(lval) * (v->count - i - 1));

    // decrease
    v->cout--;

    // realloc
    v->cell = realloc(v->cell, sizeof(lval) * v->count);
    return x;
}

// get one and remove all
lval lval_take(lval v, int i)
{
    lval x = lval_pop(v, i);
    lval_del(v);
    return x;
}

void _lval_print(lval v)
{
    switch(v->type)
    {
        case LVAL_NUM:   printf("%li", v->num); break;
        case LVAL_ERR:   printf("Error: %s", v->err); break;
        case LVAL_SYM:   printf("%s", v->sym); break;
        case LVAL_SEXPR: 
            putchar('(');
            for (int i = 0; i < v->count; ++i)
            {
                _lval_print(v->cell[i]);
                if (i != (v->count - 1))
                    putchar(' ');
            }
            putchar(')');
            break;
        default:
            break;
    }
}

// a wrapper for recursive _lval_print
void lval_print(lval v)
{
    _lval_print(v);
    putchar('\n');
}


// *************** Read AST (IO part) *****************
lval lval_read(mpc_ast_t *t)
{
    // if symbol or number, return the conversion to that type
    if (strstr(t->tag, "number"))
        return lval_read_num(t);
    if (strstr(t->tag, "symbol"))
        return lval_read_sym(t->contents);

    // If root (>) or sexpr then create empty list
    lval x = NULL;
    if(strcmp(t->tag, ">") == 0)
        x = lval_read_sexpr();
    if(strstr(t->tag, "sexpr"))
        x = lval_read_sexpr();
    
    // fill this list with any valid expression contained
    for (int i = 0; i < t->children_num; ++i){
        if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
        if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
        if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
        x = lval_add(x, lval_read(t->children[i]));
    }

    return x;
}

lval lval_read_num(long x){
    lval v = (lval)malloc(sizeof(struct LispVal));
    v->type = LVAL_NUM;
    v->num = x;
}

lval lval_read_sym(char *sym){
    lval v = (lval)malloc(sizeof(struct LispVal));
    v->type = LVAL_SYM;
    v->sym = (char*)malloc(strlen(sym) + 1);
    strcpy(v->sym, sym);
    return v;
}

lval lval_read_sexpr(void){
    lval v = (lval)malloc(sizeof(struct LispVal));
    v->type = LVAL_SEXPR;
    v->count = 0;
    v->cell = NULL;
    return v;
}

lval lval_read_err(char *msg){
    lval v = (lval)malloc(sizeof(struct LispVal));
    v->type = LVAL_ERR;
    v->err = malloc(strlen(msg)+1);
    strcpy(v->err, msg);
    return v;
}