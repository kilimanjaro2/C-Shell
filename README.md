Makefile is provided
Executable: shell

Authors : Arun H Garimella -> 201564134
          Nikhil Tadigopulla -> 201502133

Builtin commands supported: cd, pwd, echo, ls
Can execute foreground and background programs

C Files:
=======

- main.c:       contains the main() function
- shell.c:      controls the main shell loop
- input.c:      handles input for the shell
- exec.c:       executes commands sent by shell
- builtin.c:    contains the builtin functions
