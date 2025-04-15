#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_PIPE 32

struct pipeline
{
    int comno;
    int state;
    struct command *cmd[MAX_PIPE];
};

void init_pipline(struct pipeline *p);

void set_state(struct pipeline *p, int sstate);

void insert_command(struct pipeline *p, struct command *cmd);

void free_pipeline(struct pipeline *p);

void print_pipeline(struct pipeline *p);

int execute_pipeline(struct pipeline *p);