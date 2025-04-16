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
        free(c->pipes[i]);
        c->pipes[i] = NULL;
    }
}

void print_conditional_cmd(struct conditional_cmd *c)
{
    printf("Conditional Commands : %d :\n", c->pipno);
    printf("***********************************************\n");
    for (int i = 0; i < c->pipno; i++)
    {
        print_pipeline(c->pipes[i]);
        if (i < c->pipno - 1)
        {
            if (c->pipes[i]->state == 1)
            {
                printf(" || ");
            }
            else
            {
                printf(" && ");
            }
        }
        printf("***********************************************\n");
    }
    printf("Background: %s\n", c->background ? "Yes" : "No");
}

int execute_conditional_cmd(struct conditional_cmd *c)
{
    if (c == NULL)
    {
        return 0;
    }
    int status = 0;
    for (int i = 0; i < c->pipno; i++)
    {
        int cstatus = execute_pipeline(c->pipes[i]);
        status = cstatus;
        if (cstatus == 0)
        {
            // success
            if (c->pipes[i]->state == 1)
            { // OR
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            // failure
            // skip any AND commands
            while (i < c->pipno)
            {
                if (c->pipes[i]->state == 1)
                {
                    break;
                }
                else
                {
                    i++;
                }
            }
        }
    }
    return status;
}
