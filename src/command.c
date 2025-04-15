#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "command.h"

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
    }
}

int execute_command(struct command *cmd)
{
    // you should create pipe here to know if your child process failed or not
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork :(");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        execvp(cmd->args[0], cmd->args);
        fprintf(stderr, "Error: Command execution failed\n");
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    return 0;
}

void print_command(struct command *cmd)
{
    printf("Command: ");
    for (int i = 0; i < cmd->argno; ++i)
    {
        printf("%s ", cmd->args[i]);
    }
    printf(" : ");
    if (cmd->input != NULL)
        printf("Input: %s - ", cmd->input);
    if (cmd->output != NULL)
        printf("Output: %s - ", cmd->output);
    if (cmd->error != NULL)
        printf("Error: %s - ", cmd->error);
}