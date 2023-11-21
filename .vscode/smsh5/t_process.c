#include <stdio.h>
#include "smsh.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
int builtin_command(char **, int *);

int process(char **args)
{
    int rv = 0;

    int background = 0;
    if (strcmp(args[0], "&") == 0)
        background = 1;

        rv = 0;
    else if (is_control_command(args[0]))
        rv = do_control_command(args);
    else if (ok_to_execute())
        if (!builtin_command(args, &rv))
            rv = execute(args, background);

    return rv;
}