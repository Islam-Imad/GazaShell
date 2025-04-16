#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "path.h"
// #include "pipe.h"

#ifndef CONDITIONAL_CMD_H
#define CONDITIONAL_CMD_H

#define MAX_CMD 32

struct conditional_cmd
{
    int pipno;
    int background;
    struct pipeline *pipes[MAX_CMD];
};

void init_ccmd(struct conditional_cmd *c);

void set_background(struct conditional_cmd *c, int bg);

void insert_pipeline(struct conditional_cmd *c, struct pipeline *p);

void free_conditional_cmd(struct conditional_cmd *c);

void print_conditional_cmd(struct conditional_cmd *c);

int execute_conditional_cmd(struct conditional_cmd *c, struct path *p);

#endif // CONDITIONAL_CMD_H