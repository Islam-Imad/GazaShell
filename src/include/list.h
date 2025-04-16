#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef LIST_H
#define LIST_H

#define MAX_CCMD 32

struct list
{
    int ccmdno;
    struct conditional_cmd *ccmds[MAX_CCMD];
};

void init_list(struct list *l);

void insert_ccmd(struct list *l, struct conditional_cmd *c);

void free_list(struct list *l);

void print_list(struct list *l);

int parallel_execute(struct conditional_cmd *c);

int execute_list(struct list *l);

#endif