#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "varlib.h"

int execute(char *argv[])
{
    extern char **environ;

    int pid;
    int child_info = -1;
    int bg = 0;

    if (argv[0] == NULL)
        return 0;

    int i = 0;
    while (argv[i] != NULL && argv[i][0] != '\0')
    {
        if (strchr(argv[i], '&') != NULL)
        {
            bg = 1;
            argv[i] = NULL;
            break;
        }
        i++;
    }

    if ((pid = fork()) == -1)
        perror("fork");
    else if (pid == 0)
    {
        environ = VLtable2environ();
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("cannot execute command");
        exit(1);
    }
    else
    {
        if (bg == 0)
        {
            if (wait(&child_info) == -1)
                perror("wait");
        }

        bg = 0;
    }

    return child_info;
}