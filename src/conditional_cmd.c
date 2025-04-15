#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "conditional_cmd.h"
#include "pipe.h"

void init_ccmd(struct conditional_cmd *c)
{
    c->pipno = 0;
    c->background = 0;
    for (int i = 0; i < MAX_CMD; i++)
    {
        c->pipes[i] = NULL;
    }
}

void set_background(struct conditional_cmd *c, int bg)
{
    c->background = bg;
}

void insert_pipeline(struct conditional_cmd *c, struct pipeline *p)
{
    if (c->pipno < MAX_CMD)
    {
        c->pipes[c->pipno++] = p;
    }
    else
    {
        fprintf(stderr, "Error: Maximum number of pipelines reached.\n");
        exit(EXIT_FAILURE);
    }
}

void free_conditional_cmd(struct conditional_cmd *c)
{
    for (int i = 0; i < c->pipno; i++)
    {
        free_pipeline(c->pipes[i]);
    }
}

void print_conditional_cmd(struct conditional_cmd *c)
{
    printf("Conditional Command:\n");
    for (int i = 0; i < c->pipno; i++)
    {
        print_pipeline(c->pipes[i]);
    }
    printf("Background: %s\n", c->background ? "Yes" : "No");
}

int execute_conditional_cmd(struct conditional_cmd *c)
{
    for (int i = 0; i < c->pipno; i++)
    {
        if (execute_pipeline(c->pipes[i]) == -1)
        {
            // fprintf(stderr, "Error executing pipeline %d\n", i);
            // return -1;
            if (c->pipes[i]->state == 0)
            {
                // continue only if the pipeline fails
                return -1;
            }
            else if (c->pipes[i]->state == 1)
            {
                // continue only if the pipeline succeeds
                continue;
            }
            else
            {
                fprintf(stderr, "Error: Invalid pipeline state.\n");
                return -1;
            }
        }
        else
        {
            // fprintf(stderr, "Pipeline %d executed successfully\n", i);
            if (c->pipes[i]->state == 1)
            {
                // continue only if the pipeline fails
                return -1;
            }
            else if (c->pipes[i]->state == 0)
            {
                // continue only if the pipeline succeeds
                continue;
            }
            else
            {
                fprintf(stderr, "Error: Invalid pipeline state.\n");
                return -1;
            }
        }
    }
    return 0;
}
