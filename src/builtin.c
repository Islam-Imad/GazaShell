#include <stdio.h>
#include "builtin.h"

void execute_cd(char *path)
{
    if (chdir(path) != 0)
    {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
}

void excute_exit()
{
    exit(EXIT_SUCCESS);
}

void execute_help()
{
    char help_message[100] = "This is a simple shell program.\n";
    write(STDOUT_FILENO, help_message, strlen(help_message));
}

int is_builtin(struct command *cmd)
{
    for (int i = 0; i < count_builtin; i++)
    {
        if (strcmp(cmd->args[0], builtin_str[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void excute_builtin(struct command *cmd)
{
    if (strcmp(cmd->args[0], "cd") == 0)
    {
        execute_cd(cmd->args[1]);
    }
    else if (strcmp(cmd->args[0], "exit") == 0)
    {
        excute_exit();
    }
    else if (strcmp(cmd->args[0], "help") == 0)
    {
        execute_help();
    }
}