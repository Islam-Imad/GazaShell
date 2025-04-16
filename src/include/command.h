#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "path.h"

#ifndef COMMAND_H
#define COMMAND_H

#define MAX_ARGS 256

struct command
{
    char *input;
    char *output;
    char *error;
    int argno;
    char *args[MAX_ARGS];
};

void init_command(struct command *cmd);

void set_input(struct command *cmd, char *infile);

void set_output(struct command *cmd, char *outfile);

void set_error(struct command *cmd, char *erfile);

void insert_arg(struct command *cmd, char *arg);

void free_command(struct command *cmd);

int execute_command(struct command *cmd, struct path *p);

void print_command(struct command *cmd);

#endif // COMMAND_H