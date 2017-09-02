#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "builtin.h"
#include "exec.h"
#include "input.h"
#include "shell.h"

int exit_shell = 0;
char* home_dir = NULL;

extern int errno;

void printPrompt();
void printDirectory();

void printError()
{
    fprintf(stderr, "Exit: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

void runShell()
{
    errno = 0;
    home_dir = getcwd(NULL, 0);

    if(home_dir == NULL)
        printError();

    int i, j, k;
    char ***commands;

    while(1)
    {
        printPrompt();
        commands = processInput();

        for(i=0; commands[i]; ++i)
        {
            int done = 0;

            for(k=0; k<num_builtins(); ++k)
            {
                if(strcmp(commands[i][0], builtin_str[k]) == 0)
                {
                    (builtin_func[k])(commands[i]);
                    done = 1;
                }
                if(done == 1)
                    break;
            }

            if(done == 1)
                continue;

            int bg = 0;
            for(j=0; commands[i][j]; ++j);

            if(strcmp(commands[i][j-1], "&") == 0)
            {
                free(commands[i][j-1]);
                commands[i][j-1] = NULL;

                bg = 1;
            }

            executeCommand(commands[i], bg);
        }

        for(i=0; commands[i]; ++i)
        {
            for(j=0; commands[i][j]; ++j)
                free(commands[i][j]);
            free(commands[i]);
        }
        free(commands);

        if(exit_shell)
            break;
    }
}

void printPrompt()
{
    struct passwd *user = getpwuid(getuid());

    if(user == NULL)
        printError();

    printf("<%s@", user->pw_name);

    struct utsname system;
    if(uname(&system) == -1)
    {
        printError();
    }

    printf("%s:", system.nodename);

    printDirectory();
}

void printDirectory()
{
    char* cur_dir = NULL;
    cur_dir = getcwd(NULL, 0);

    if(cur_dir == NULL)
        printError();

    int inside_home = 1;

    int i;
    for(i=0; home_dir[i] && inside_home; ++i)
        if(home_dir[i] != cur_dir[i])
            inside_home = 0;

    if(inside_home)
        printf("~%s> ", cur_dir+i);
    else
        printf("%s> ", cur_dir);
}
