/*
Aim to build a command interpreter
A prototype now
*/
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

// >>>>>>>>>>> Header <<<<<<<<<<<<<<<<<
#define MAX_BUF_LEN 2048

int cmd_handle(char *input);
void check_result(int result);


// >>>>>>>>>>>> Implement <<<<<<<<<<<<
int cmd_handle(char *input)
{
    // just passed now
    fprintf(stdout, "OK I got the input '%s' but do nothing now\n", input);
    if (strcmp("wrong", input) == 0)
    {
        printf("Oh We cannot exec 'wrong' :( \n");
        return 0;
    }
    return 1;
}

void check_result(int result)
{
    if (result == 0)
        fprintf(stderr, "x");
}

// >>>>>>>>>>>> Main <<<<<<<<<<<<<<<<<<<<
int main(int argc, char *argv)
{
    // prompt, can be read from config in the future
    const char *prompt = "[console]> ";
    char *cmd_buffer = NULL;
    int result = 0;

    // display the welcome info
    printf("Welcome Hereeee, Ctrl-C to quit\n");

    // A while loop to recv input
    while((cmd_buffer = readline(prompt)) != NULL)
    {
        if (strlen(cmd_buffer) > 0)
            add_history(cmd_buffer);

        // relay to handler
        result = cmd_handle(cmd_buffer);

        // check result
        check_result(result);
    }

    return 0;
}