#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "path.h"

void init_path(struct path *p)
{
    p->pathno = 0;
    for (int i = 0; i < MAX_PATH; i++)
    {
        p->path[i] = NULL;
    }
}

void insert_path(struct path *p, char *path)
{
    if (p->pathno >= MAX_PATH)
    {
        fprintf(stderr, "Path limit reached\n");
        return;
    }
    p->path[p->pathno] = strdup(path);
    p->pathno++;
}

void free_path(struct path *p)
{
    for (int i = 0; i < p->pathno; i++)
    {
        if (p->path[i] != NULL)
            free(p->path[i]);
        p->path[i] = NULL;
    }
    free(p);
}

void clear_path(struct path *p)
{
    for (int i = 0; i < p->pathno; i++)
    {
        free(p->path[i]);
        p->path[i] = NULL;
    }
    p->pathno = 0;
}

void print_path(struct path *p)
{
    for (int i = 0; i < p->pathno; i++)
    {
        printf("%s\n", p->path[i]);
    }
}

int check_path(char *p)
{
    if (access(p, F_OK) == 0 && access(p, X_OK) == 0)
    {
        return 1; // Path exists
    }
    return 0; // Path does not exist
}

char *get_path(struct path *p, char *name)
{
    char *full_path = NULL;
    for (int i = 0; i < p->pathno; ++i)
    {
        char temp[1024] = ""; // Use a character array with sufficient size
        strcat(temp, p->path[i]);
        strcat(temp, "/");
        strcat(temp, name);
        if (check_path(temp)) // Pass the correct argument
        {
            full_path = strdup(temp);
            break;
        }
    }
    return full_path;
}