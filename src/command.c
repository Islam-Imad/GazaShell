#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "command.h"
#include "builtin.h"
#include "path.h"

void init_command(struct command *cmd)
{
    cmd->argno = 0;
    cmd->input = NULL;
    cmd->output = NULL;
    cmd->error = NULL;
    for (int i = 0; i < MAX_ARGS; ++i)
    {
        cmd->args[i] = NULL;
    }
}

void set_input(struct command *cmd, char *infile)
{
    cmd->input = strdup(infile);
}

void set_output(struct command *cmd, char *outfile)
{
    cmd->output = strdup(outfile);
}

void set_error(struct command *cmd, char *erfile)
{
    cmd->error = strdup(erfile);
}

void insert_arg(struct command *cmd, char *arg)
{
    if (cmd->argno < MAX_ARGS)
    {
        cmd->args[cmd->argno] = strdup(arg);
        cmd->argno += 1;
    }
    else
    {
        fprintf(stderr, "Error: Too many arguments\n");
        exit(EXIT_FAILURE);
    }
}

void free_command(struct command *cmd)
{
    if (cmd->input != NULL)
        free(cmd->input);
    if (cmd->output != NULL)
        free(cmd->output);
    if (cmd->error != NULL)
        free(cmd->error);
    for (int i = 0; i < cmd->argno; ++i)
    {
        free(cmd->args[i]);
        cmd->args[i] = NULL;
    }
}

int execute_command(struct command *cmd, struct path *p)
{
    if (cmd == NULL)
    {
        return 0;
    }
    if (cmd->argno == 0)
    {
        if (cmd->output != NULL || cmd->input != NULL || cmd->error != NULL)
        {
            error_message();
            return -1;
        }
        return 0;
    }
    // you should create pipe here to know if your child process failed or not
    if (check_builtin(cmd))
    {
        excute_builtin(cmd, p);
        return 0;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork :(");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        char *path = get_path(p, cmd->args[0]);
        if (path != NULL)
        {
            execv(path, cmd->args);
        }
        error_message();
        // printf("Command not found: %s\n", cmd->args[0]);
        // // print the env path
        // printf("%s\n", getenv("PATH"));
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    return 0;
}

void print_command(struct command *cmd)
{
    printf("( Command: ");
    for (int i = 0; i < cmd->argno; ++i)
    {
        printf("%s ", cmd->args[i]);
    }
    printf("  [");
    if (cmd->input != NULL)
        printf("In: %s - ", cmd->input);
    else
        printf("In: NULL - ");
    if (cmd->output != NULL)
        printf("Out: %s - ", cmd->output);
    else
        printf("Out: NULL - ");
    if (cmd->error != NULL)
        printf("Er: %s ", cmd->error);
    else
        printf("Er: NULL ");

    printf("] ) ");
}