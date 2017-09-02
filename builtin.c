#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "builtin.h"
#include "shell.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <grp.h>
#include <time.h>
#include <pwd.h>
#include <sys/stat.h>

#define DEF "\x1B[0m"
// #define for_each_process(p) for(p = &init_task ; (p = next_task(p)) != &init_task ;);

char *builtin_str[] =
{
	"cd",
	"pwd",
	"echo",
	"pinfo",
	"ls",
  "nightswatch"
};


void (*builtin_func[]) (char**) =
{
	&builtin_cd,
	&builtin_pwd,
	&builtin_echo,
	&builtin_pinfo,
	&builtin_ls,
  &builtin_nightswatch
};

int num_builtins()
{
	return (sizeof(builtin_str) / sizeof(char*));
}

void builtin_cd(char** arguments)
{
	if(arguments[1] == NULL)
	{
		if(chdir(home_dir) == -1)
			printError();
	}
	else if(chdir(arguments[1]) == -1)
		printError();
}

void builtin_pwd(char** arguments)
{
	char* cur_dir = NULL;
	cur_dir = getcwd(NULL, 0);

	if(cur_dir == NULL)
		printError();

	printf("%s\n", cur_dir);
}

void builtin_nightswatch(char** arguments)
{
  int i = 1;
}

void builtin_echo(char** arguments)
{
	int i=1;
	while(arguments[i])
	{
		if(i-1)
			printf(" ");
		printf("%s", arguments[i]);
		i++;
	}
	printf("\n");
}

void  builtin_ls(char** arguments)
{
	int flag = 1;
	//printf("%s\n",arguments[1] );
	int curr = 1;
	while(arguments[curr] != NULL)
	{
		if(strcmp(arguments[curr],"-la") == 0 || strcmp(arguments[curr],"-al") == 0)
			flag = 6;
		else if(strcmp(arguments[curr],"-l") == 0 && flag%3)
			flag *= 3;
		else if(strcmp(arguments[curr],"-a") == 0 && flag%2)
			flag *= 2;
      curr++;
	}
  int total = 0,i=0;
  struct dirent **listr;
  struct stat details;
  char timer[14];
  int listn = scandir(".", &listr, 0, alphasort);
	//  printf("%d",flag);
	if(flag == 1 || flag == 2)
	{
		if (listn >= 0)
		{
			for(i = 0; i < listn; i++ )
			{
				if((strcmp(listr[i]->d_name,".") == 0 || strcmp(listr[i]->d_name,"..") == 0))
        {
          if(flag == 1)
            continue;
          else
					     printf("%s    ", listr[i]->d_name);
        }
        else
					printf("%s    ", listr[i]->d_name);
				if(!(i%5))
					printf("\n");
			}
			printf("\n");
		}
		else
			printError();
	}
  else
  {
    if(listn >= 0)
    {
        if(flag%2 == 0)
          printf("Total %d\n", listn);
        else
          printf("Total %d\n", listn -2);
        for ( i = 0; i < listn; i++)
        {
            if(((strcmp(listr[i]->d_name,".")==0 || strcmp(listr[i]->d_name,"..")==0) && (flag%2 == 0 && (stat(listr[i]->d_name,&details)==0))) || (stat(listr[i]->d_name,&details)==0))
            {
                  total += details.st_blocks; // block size
                  printf("%s%1s",DEF,(S_ISDIR(details.st_mode)) ? "d" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IRUSR) ? "r" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IWUSR) ? "w" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IXUSR) ? "x" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IRGRP) ? "r" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IWGRP) ? "w" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IXGRP) ? "x" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IROTH) ? "r" : "-");
                  printf("%s%1s",DEF,(details.st_mode & S_IWOTH) ? "w" : "-");
                  printf("%s%1s ",DEF,(details.st_mode & S_IXOTH) ? "x" : "-");
                  // links associated - owner name - group name
                  printf("%2ld ",(unsigned long)(details.st_nlink));
                  printf("%s ",(getpwuid(details.st_uid))->pw_name);
                  printf("%s ",(getgrgid(details.st_gid))->gr_name);
                  // file size (bytes) - time modified - name
                  printf("%5lld ",(unsigned long long)details.st_size);
                  strftime (timer,14,"%h %d %H:%M",localtime(&details.st_mtime));
                  printf("%s ",timer);
                  printf("%s    ", listr[i]->d_name);
                  printf("\n");
            }
        }
    }
  }

}

void builtin_pinfo(char** arguments)
{
	int c, len=0;
	pid_t cpid;

	if(arguments[1] == NULL)
		cpid = getpid();
	else
		cpid = atoi(arguments[1]);

	char path[256];
	char buf[256];

	printf("pid -- %d\n", cpid);

	sprintf(path, "/proc/%d/status", cpid);
	FILE* fp = fopen(path, "r");
	if(!fp)
    printError();


	char state;
	fgets(buf,256, fp);
	fgets(buf, 256, fp);
	sscanf(buf, "State:\t%c", &state);
	printf("Process Status -- %c\n", state);
	fclose(fp);


	sprintf(path, "/proc/%d/statm", cpid);
	fp = fopen(path, "r");
	if(!fp)
		printError();


		while((c=fgetc(fp))!=' ')
			buf[len++] = c;
		buf[len] = '\0';
		printf("Memory -- %s\n", buf);
		fclose(fp);


	sprintf(path, "/proc/%d/exe", cpid);
	len = -1;
	len = readlink(path, buf, 255);
	if(len==-1)
		printError();


		buf[len] = '\0';
		int i, j;
		for(i = 0; buf[i]==home_dir[i] && buf[i]!='\0' && home_dir[i] != '\0'; i++);
		if(home_dir[i] == '\0')
		{
			buf[0] = '~';
			for(j = 1; buf[i]!='\0'; j++)
			{
				buf[j] = buf[i++];
			}
			buf[j] = '\0';
		}
		printf("Executable Path -- %s\n", buf);

}
