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

int is_file(char *path)
{
    int idx = 0;
    while (path[idx] != '\0')
    {
        if (path[idx] == '.')
        {
            return 1;
        }
        idx++;
    }
    return 0;
}

char *get_pwd()
{
    char *path = getcwd(NULL, 0);
    if (path == NULL)
    {
        error_message();
        return NULL;
    }
    return path;
}

int execute_pwd(struct command *cmd)
{
    if (cmd->argno != 1)
    {
        error_message();
        return -1;
    }
    char *path = get_pwd();
    if (path != NULL)
    {
        printf("%s\n", path);
        free(path);
        return 0;
    }
    return -1;
}

int execute_cd(struct command *cmd)
{
    if (cmd->argno != 2)
    {
        error_message();
        return -1;
    }
    if (chdir(cmd->args[1]) != 0)
    {
        error_message();
        return -1;
    }
    return 0;
}

int excute_exit(struct command *cmd)
{
    if (cmd->argno != 1)
    {
        error_message();
        return -1;
    }
    exit(EXIT_SUCCESS);
    return 0;
}

int execute_help(struct command *cmd)
{
    if (cmd->argno != 1)
    {
        error_message();
        return -1;
    }
    char help_message[100] = "This is a simple shell program.\n";
    write(STDOUT_FILENO, help_message, strlen(help_message));
    return 0;
}

int execute_path(struct command *cmd, struct path *p)
{
    clear_path(p);
    for (int i = 1; i < cmd->argno; ++i)
    {
        insert_path(p, cmd->args[i]);
    }
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

int excute_builtin(struct command *cmd, struct path *p)
{
    if (strcmp(cmd->args[0], "cd") == 0)
    {
        return execute_cd(cmd);
    }
    else if (strcmp(cmd->args[0], "exit") == 0)
    {
        return excute_exit(cmd);
    }
    else if (strcmp(cmd->args[0], "help") == 0)
    {
        return execute_help(cmd);
    }
    else if (strcmp(cmd->args[0], "pwd") == 0)
    {
        return execute_pwd(cmd);
    }
    else if (strcmp(cmd->args[0], "path") == 0)
    {
        return execute_path(cmd, p);
    }
    else
    {
        error_message();
    }
    return -1;
}