#ifndef BUILTIN
#define BUILTIN

int num_builtins();
void builtin_cd(char**);
void builtin_pwd(char**);
void builtin_echo(char**);
void builtin_pinfo(char**);
void builtin_ls(char**);
void builtin_nightswatch(char**);
void builtin_setenv(char**);
void builtin_unsetenv(char**);
void builtin_jobs(char**);
void builtin_overkill(char**);
void builtin_fg(char**);

extern char *builtin_str[];
extern void (*builtin_func[]) (char**);

#endif
