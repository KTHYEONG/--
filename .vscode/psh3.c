#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 20
#define ARGLEN 100

int excute(char **);
char *makestring(char *);

int main()
{
    char *arglist[MAXARGS + 1];
    int numargs;
    char argbuf[ARGLEN];
    char *makestring();

    numargs = 0;
    while (numargs < MAXARGS)
    {
        printf("Arg[%d]? ", numargs);
        if (fgets(argbuf, ARGLEN, stdin) == NULL)
            break;
        else
        {
            if (strcmp(argbuf, "exit\n") == 0)
                break;
            else if (*argbuf != '\n')
                arglist[numargs++] = makestring(argbuf);
            else
            {
                if (numargs > 0)
                {
                    arglist[numargs] = NULL;
                    excute(arglist);
                    numargs = 0;
                }
            }
        }
    }

    return 0;
}

int excute(char *arglist[])
{
    int pid, exitstatus;

    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        exit(1);
        break;
    case 0:
        execvp(arglist[0], arglist);
        perror("execvp failed");
        exit(1);
    default:
        while (wait(&exitstatus) != pid)
            ;
        printf("child exited with status %d, %d\n", exitstatus >> 8, exitstatus & 0377);
        break;
    }
}

char *makestring(char *buf)
{
    char *cp;

    buf[strlen(buf) - 1] = '\0';
    cp = malloc(strlen(buf) + 1);
    if (cp == NULL)
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}