#include "cmd.h"

/*
* From my perspactive, cmd is a special lang
* that means we can also create a language for it
*/

// >>>>>> Static <<<<<<<<
static char *help_msg()
{
    const char *msg = 
    "[+]Use #[lang] to switch, support:\n\
     \t[1]lisp\n\t[2]cmd\n\
[+]Cmd support:\n\
     \t[1]help: print this message\n\
     \t[2]version: version info\n\
     \t[3]info <msg>: echo message\n";
    
    char *v;
    int len;

    len = strlen(msg);
    v = (char *)malloc(len + 1);
    strcpy(v, msg);
    v[len] = '\0';

    return v;
}

static char *version_msg()
{
    const char *msg = "[+]version 0.2\n";
    char *v;
    int len;

    len = strlen(msg);
    v = (char *)malloc(len + 1);
    strcpy(v, msg);
    v[len] = '\0';

    return v;
}

static char *info(char *msg)
{
    char *v;
    int len;

    len = strlen(msg);
    v = (char *)malloc(len + 1);
    strcpy(v, msg);
    v[len] = '\0';

    return v;
}

static cmdval cmd_init_val(int argc)
{
    cmdval v;
    v = (cmdval)malloc(sizeof(struct CmdValStruct));
    v->type = CMD_ERR;
    v->result = NULL;
    v->argc = argc;
    v->argv = (char **)malloc(sizeof(char *) * argc);
    return v;
}

static cmdval cmd_error(char *msg)
{
    cmdval v;
    v = cmd_init_val(0);

    int size = strlen(msg) + 1;
    v->result = (char *)malloc(size);
    strcpy(v->result, msg);
    v->result[size] = '\0';

    return v;
}

static cmdval _cmd_eval(cmdval v)
{
    char *cmd_name;
    cmd_name = v->argv[0];

    if(strcmp(cmd_name, "help") == 0)
    {
        v->type = CMD_RES;
        v->result = help_msg();
        return v;
    }

    if(strcmp(cmd_name, "version") == 0)
    {
        v->type = CMD_RES;
        v->result = version_msg();
        return v;
    }

    if(strcmp(cmd_name, "info") == 0)
    {
        if(v->argc != 2)
        {
            cmd_del(v);
            return cmd_error("Info required one argument");
        }
        
        v->result = info(v->argv[1]);
        v->type = CMD_RES;
        return v;
    }

    return cmd_error("Unknown Eval Here");
}


/// >>>>>> Expose Function <<<<<<<<<
// 1. support init
LangParser init_cmd(void)
{
    LangParser cmd_parser;

    cmd_parser = init_parser(2);
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
    
    // prepare lang
    cmd_parser->lang = Cmd;
    cmd_parser->rule[0] = Name;
    cmd_parser->rule[1] = Arg;

    return cmd_parser;
}

// 2. support clean
void clean_cmd(LangParser parser)
{
    mpc_cleanup(
        3,
        parser->lang,
        parser->rule[0],
        parser->rule[1]
    );

    clean_parser(parser);
}

// 3. support read from input 
//    trans AST tree into args
cmdval cmd_read(mpc_ast_t *t)
{
    cmdval v;
    int argc;
    char *tmp;
    int tmp_size;

    // should be root node, do not recur
    if (strcmp(t->tag, ">") != 0)
    {
        v = cmd_error("Dunnot support recursive cmd");
        return v;
    }

    // pack info
    argc = t->children_num;
    v = cmd_init_val(argc);

    // first one must be cmd
    // or use strstr("cmd_name" to find it)
    // first and last is empty, drop them
    for (int i = 1; i < argc - 1; ++i)
    {
        tmp = t->children[i]->contents;
        tmp_size = strlen(tmp) + 1;
        v->argv[i-1] = (char *)malloc(tmp_size);
        strcpy(v->argv[i-1], tmp);
        v->argv[i-1][tmp_size] = '\0';
    }
    v->argc = argc - 2;
    v->type = CMD_NO;

    return v;
}

// 4. support eval
cmdval cmd_eval(cmdval v)
{
    switch(v->type)
    {
        case CMD_ERR:
        case CMD_RES:
            return v;
        
        case CMD_NO:
            return _cmd_eval(v);    // true exec

        default:
            printf("[x]Weird Things Happend\n");
            cmd_del(v);
            return cmd_error("Weird");
    }
}



// 5. support prinln
void cmd_println(cmdval v)
{
    switch(v->type)
    {
        case CMD_ERR:
            printf("[x]Error!\n");
            printf("\t%s", v->result);
            break;
        case CMD_RES:
            printf("%s\n", v->result);
            break;
        case CMD_NO:
        default:
            printf("[?]impossible happend\n");
            break;
    }
    puts("\n");
}

// 6. support del
void cmd_del(cmdval v)
{
    if (v->result != NULL)
        free(v->result);
    
    for (int i = 0; i < v->argc; ++i)
        if(v->argv[i] != NULL)
            free(v->argv[i]);

    free(v->argv);
    free(v);
}

