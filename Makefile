all: shell

shell: main.c
shell: input.c
shell: shell.c
shell: exec.c
shell: builtin.c
	gcc -o $@ $^

clean:
	rm shell
