#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtin.h"

const char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "path",
    "pwd"};

const int count_builtin = 5;

void error_message()
{
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

int execute_pwd()
{
    char *path = getcwd(NULL, 0);
    if (path == NULL)
    {
        error_message();
        return -1;
    }
    printf("%s\n", path);
    free(path);
}

int execute_cd(char *path)
{
    if (chdir(path) != 0)
    {
        error_message();
        return -1;
    }
    return 0;
}

int excute_exit()
{
    exit(EXIT_SUCCESS);
    return 0;
}

int execute_help()
{
    char help_message[100] = "This is a simple shell program.\n";
    write(STDOUT_FILENO, help_message, strlen(help_message));
    return 0;
}

int check_builtin(struct command *cmd)
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

int excute_builtin(struct command *cmd)
{
    if (strcmp(cmd->args[0], "cd") == 0)
    {
        return execute_cd(cmd->args[1]);
    }
    else if (strcmp(cmd->args[0], "exit") == 0)
    {
        return excute_exit();
    }
    else if (strcmp(cmd->args[0], "help") == 0)
    {
        return execute_help();
    }
    else if (strcmp(cmd->args[0], "pwd") == 0)
    {
        return execute_pwd();
    }
    return -1;
}