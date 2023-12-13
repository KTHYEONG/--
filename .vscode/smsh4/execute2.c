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

    if (*(argv[0] + strlen(argv[0]) - 1) == '&')
    {
        bg = 1;
        *(argv[0] + strlen(argv[0] - 1)) = '\0';
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