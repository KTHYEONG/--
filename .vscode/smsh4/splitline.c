#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

#define is_delim(x) ((x) == ' ' || (x) == '\t')

int cnt = 0;

char *next_cmd(char *prompt, FILE *fp)
{
    char *buf;
    int bufspace = 0;
    int pos = 0;
    int c;

    if (cnt == 0)
        printf("%s", prompt);
    while ((c = getc(fp)) != EOF)
    {
        if (c == ';')
        {
            cnt++;
            break;
        }

        if (pos + 1 >= bufspace)
        {
            if (bufspace == 0)
                buf = emalloc(BUFSIZ);
            else
                buf = eralloc(buf, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
        }

        if (c == '\n')
        {
            cnt = 0;
            break;
        }

        buf[pos++] = c;
    }
    if (c == EOF && pos == 0)
        return NULL;
    buf[pos] = '\0';
    return buf;
}

char **splitline(char *line)
{
    char *newstr();
    char **args;
    int spots = 0;
    int bufspace = 0;
    int argnum = 0;
    char *cp = line;
    char *start;
    int len;

    if (line == NULL)
        return NULL;

    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char *);
    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char *);

    while (*cp != '\0')
    {
        while (is_delim(*cp))
            cp++;

        if (*cp == '\0')
            break;

        if (argnum + 1 >= spots)
        {
            args = eralloc(args, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ / sizeof(char *));
        }

        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delim(*cp)))
            len++;
        args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;
    return args;
}

char *newstr(char *s, int l)
{
    char *rv = emalloc(l + 1);

    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char **list)
{
    char **cp = list;
    while (*cp)
        free(*cp++);
    free(list);
}

void *emalloc(size_t n)
{
    void *rv;
    if ((rv = malloc(n)) == NULL)
        fatal("out of memory", "", 1);
    return rv;
}

void *eralloc(void *p, size_t n)
{
    void *rv;
    if ((rv = realloc(p, n)) == NULL)
        fatal("realloc() failed", "", 1);
    return rv;
}