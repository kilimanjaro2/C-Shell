#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"
#include "shell.h"

void executeCommand(char** arguments, int bg)
{
    pid_t child_pid;

    child_pid = fork();

    if(child_pid == -1)
        printError();

    if(child_pid == 0)
    {
        if(bg == 1)
            setpgid(0, 0);
        if(execvp(arguments[0], arguments) == -1)
            printError();
    }
    else if(bg != 1)
    {
          int status;
          if(waitpid(child_pid, &status, 0) == -1)
              printError();
    }


}
