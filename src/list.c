#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "list.h"
#include "conditional_cmd.h"

void init_list(struct list *l)
{
    l->ccmdno = 0;
    for (int i = 0; i < MAX_CCMD; i++)
    {
        l->ccmds[i] = NULL;
    }
}

void insert_ccmd(struct list *l, struct conditional_cmd *c)
{
    if (l->ccmdno < MAX_CCMD)
    {
        l->ccmds[l->ccmdno++] = c;
    }
    else
    {
        fprintf(stderr, "Error: Maximum number of conditional commands reached.\n");
        exit(EXIT_FAILURE);
    }
}

void free_list(struct list *l)
{
    for (int i = 0; i < l->ccmdno; i++)
    {
        free_conditional_cmd(l->ccmds[i]);
        free(l->ccmds[i]);
        l->ccmds[i] = NULL;
    }
}

void print_list(struct list *l)
{
    printf("List of Conditional Commands: %d :\n", l->ccmdno);
    printf("-----------------------------------------------\n");
    for (int i = 0; i < l->ccmdno; i++)
    {
        // printf("Conditional Command %d:\n", i + 1);
        print_conditional_cmd(l->ccmds[i]);
        printf("-----------------------------------------------\n");
    }
}

// still we need to know the status of the conditional command
int parallel_execute(struct conditional_cmd *c)
{
    if (c == NULL)
    {
        return 0;
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return -1;
    }
    else if (pid == 0)
    {
        // Child process
        // sleep(1);
        // printf("Executing command in background: ");
        execute_conditional_cmd(c);
        exit(0);
    }
    else
    {
        // don't wait for the child process
        // Parent process
    }
    return 0;
}

int execute_list(struct list *l)
{
    if (l == NULL)
    {
        return 0;
    }
    int status = 0;
    // printf("%d\n", l->ccmdno);
    for (int i = 0; i < l->ccmdno; i++)
    {
        // print_conditional_cmd(l->ccmds[i]);
        if (l->ccmds[i] == NULL)
            continue;
        if (l->ccmds[i]->background == 1)
        {
            parallel_execute(l->ccmds[i]);
        }
        else
        {
            int cstatus = execute_conditional_cmd(l->ccmds[i]);
            if (cstatus != 0)
            {
                status = -1;
            }
        }
        // break;
    }
    return status;
}