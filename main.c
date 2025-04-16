#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "builtin.h"
#include "path.h"

char *read_line(FILE *file)
{
    if (feof(file))
        return NULL;
    char *buffer = NULL;
    size_t len = 0;
    ssize_t read;
    read = getline(&buffer, &len, file);
    if (read == -1)
    {
        // perror("getline");
        return NULL;
    }
    return buffer;
}

int batch_mode(char *path)
{
    if (path == NULL || strlen(path) == 0)
    {
        error_message();
        exit(1);
    }
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        error_message();
        exit(1);
    }
    struct path p;
    insert_path(&p, "/bin");
    insert_path(&p, "/usr/bin");
    int status = 0;
    char *line = NULL;
    struct tokens *t;
    struct list *l;
    int occ = 0;
    while ((line = read_line(file)) != NULL)
    {
        occ++;
        t = scanner(line);
        l = parser(t);
        if (execute_list(l, &p) != 0)
        {
            status = 1;
        }
        free_list(l);
        free(line);
    }
    if (occ == 0)
    {
        error_message();
    }
    fclose(file);
    return status;
}

int command_line_mood()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    struct path p;
    insert_path(&p, "/bin");
    insert_path(&p, "/usr/bin");
    while (1)
    {
        printf("Wish> ");
        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            perror("getline");
            break;
        }
        struct tokens *t = scanner(line);
        struct list *l = parser(t);
        execute_list(l, &p);
        free_list(l);
        free(t);
    }
    print_path(&p);
    free(line);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        command_line_mood();
    }
    else if (argc == 2)
    {
        batch_mode(argv[1]);
    }
    else
    {
        error_message();
        exit(EXIT_FAILURE);
    }
    return 0;
}