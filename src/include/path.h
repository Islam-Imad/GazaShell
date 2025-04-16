#include <stdio.h>

#ifndef PATH_H
#define PATH_H

#define MAX_PATH 1024

struct path
{
    char *path[MAX_PATH];
    int pathno;
};

void init_path(struct path *p);

void insert_path(struct path *p, char *path);

void free_path(struct path *p);

void clear_path(struct path *p);

void print_path(struct path *p);

char *get_path(struct path *p, char *name);

int check_path(char *p);

#endif // PATH_H