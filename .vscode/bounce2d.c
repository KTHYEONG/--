#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MESSAGE "()"
#define BLANK "  "

void move_msg(int);
int set_ticker(int);

int row, col, col_dir, row_dir, col_speed, row_speed;

#define TOP_ROW 5
#define BOT_ROW 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70

void ignore_signals();

int main()
{
    int delay;
    int ndelay;
    int c;
    void move_msg(int);

    initscr();
    crmode();
    noecho();
    clear();

    row = 10;
    col = 0;
    col_dir = 1;
    row_dir = 1;
    col_speed = 1;
    row_speed = 1;
    delay = 20;

    ignore_signals();

    move(row, col);
    addstr(MESSAGE);
    signal(SIGALRM, move_msg);
    set_ticker(delay);

    while (1)
    {
        ndelay = 0;
        c = getch();
        if (c == 'Q')
            break;
        if (c == ' ')
            col_dir = -col_dir;
        if (c == 's')
            col_speed = (col_speed > 1) ? col_speed - 1 : 1;
        if (c == 'S')
            row_speed = (row_speed > 1) ? row_speed - 1 : 1;
        if (c == 'f')
            col_speed++;
        if (c == 'F')
            row_speed++;
        if (ndelay > 0)
            set_ticker(delay = ndelay);
    }
    endwin();
    return 0;
}

void move_msg(int signum)
{
    signal(SIGALRM, move_msg);
    move(row, col);
    addstr(BLANK);

    col += col_dir * col_speed;
    row += row_dir * row_speed;

    if (row <= TOP_ROW || row + 1 >= BOT_ROW)
        row_dir = -row_dir;

    if (col_dir == -1 && col <= LEFT_EDGE)
        col_dir = 1;
    else if (col_dir == 1 && col + strlen(MESSAGE) >= RIGHT_EDGE)
        col_dir = -1;

    move(row, col);
    addstr(MESSAGE);
    refresh();
}

int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void ignore_signals()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}