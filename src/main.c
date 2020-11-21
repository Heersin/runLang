/*
Aim to build a command interpreter
A prototype now
// TODOD : last parser won't be cleaned up
            though it causes no problem
            should keep a global var CURRENT_LANG to solve this
*/

#include "engine.h"

// >>>>>>>>>>>> Main <<<<<<<<<<<<<<<<<<<<
int main(int argc, char **argv)
{
    // prompt, can be read from config in the future
    const char *prompt = "[console]> ";
    char *cmd_buffer = NULL;
    int result = 0;
    
    // init support lang
    init_context();

    // display the welcome info
    printf("Welcome Hereeee, Ctrl-C to quit\n");

    // A while loop to recv input
    while((cmd_buffer = readline(prompt)) != NULL)
    {
        if (strlen(cmd_buffer) > 0)
            add_history(cmd_buffer);

        // relay to handler
        result = handle_cmd(cmd_buffer);

        // check result
        check_result(result);
        free(cmd_buffer);
    }

    clean_context();

    return 0;
}