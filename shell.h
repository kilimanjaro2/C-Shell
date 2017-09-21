#ifndef SHELL
#define SHELL

extern char* home_dir;

char *bg_processes[32768];
int bg_order[32768];

void printError();
void runShell();

#endif
