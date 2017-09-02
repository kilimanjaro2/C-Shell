#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "shell.h"

char*** getCommands(char*);
char** getArguments(char*, int, int);
char* copyString(int, int, char*);

char*** processInput()
{
    char *line = NULL;
    size_t len = 0;
    char ***commands;
    
    if(getline(&line, &len, stdin) != -1)
        commands = getCommands(line);
    else
        printError();
    
    free(line);
    
    return commands;
}

char*** getCommands(char *line)
{
    int com_buffer = 128;
    char ***commands = malloc(com_buffer * sizeof(char**));
    
    if(commands == NULL)
        printError();
    
    int i, com_count = 0, com_s = 0;
    for(i=0; ; ++i)
    {
        if(line[i] == ';' || line[i] == '\n' || line[i] == '\0')
        {
            if(com_s != i)
            {
                if(com_count == com_buffer)
                {
                    com_buffer *= 2;
                    commands = realloc(commands, com_buffer);
                    
                    if(commands == NULL)
                        printError();
                }
                        
                commands[com_count] = getArguments(line, com_s, i);
                com_count++;
            }
            
            if(line[i] == 0)
                break;
            
            com_s = i+1;
        }
    }
    
    commands[com_count] = NULL;
    
    return commands;
}

char** getArguments(char* line, int com_s, int com_f)
{
    int arg_buffer = 128;
    char **arguments = malloc(arg_buffer * sizeof(char*));
    
    if(arguments == NULL)
        printError();
    
    int i, arg_count = 0, arg_s = com_s;
    for(i=com_s; ; ++i)
    {
        if(line[i] == ' ' || i == com_f)
        {
            if(arg_s != i)
            {
                if(arg_count == arg_buffer)
                {
                    arg_buffer *= 2;
                    arguments = realloc(arguments, arg_buffer);
                    
                    if(arguments == NULL)
                        printError();
                }
                
                arguments[arg_count] = copyString(arg_s, i, line);
                arg_count++;
            }
            
            if(i == com_f)
                break;
            
            arg_s = i+1;
        }
    }
    
    arguments[arg_count] = NULL;
    
    return arguments;
}

char* copyString(int start, int finish, char* str)
{
    int length = finish - start + 1;
    char* str_copy = malloc(length * sizeof(char));
    
    if(str_copy == NULL)
        printError();
    
    int i;
    for(i=0; i<length-1; ++i)
        str_copy[i] = str[start+i];
    str_copy[length-1] = 0;
    
    return str_copy;
}
