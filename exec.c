#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include<stdio.h>

#include "exec.h"
#include "shell.h"

#define TOTAL 1024
int bg_len = 0;

void executeCommand(char** arguments, int bg)
{
    pid_t child_pid;

    child_pid = fork();

    if(child_pid == -1)
        printError();
    if(child_pid==0)
    {
        if(bg == 1)
        {
            setpgid(0, 0);
            bg_len++;
            bg_order[bg_len] = child_pid;
            bg_processes[child_pid] = malloc (TOTAL);
            strcpy(bg_processes[child_pid], arguments[0]);
        }
        if(execvp(arguments[0], arguments) == -1)
            printError();
    }
    else if(bg != 1)
    {
          int status;
          if(waitpid(child_pid, &status, 0) == -1)
              printError();
    }
    else if(child_pid!=0 && child_pid!=-1 && bg == 1)
    {
      bg_len++;
      bg_order[bg_len] = child_pid;
      bg_processes[child_pid] = malloc (TOTAL);
      strcpy(bg_processes[child_pid], arguments[0]);
    }
}
