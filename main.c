#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>
#include <math.h>
#include "parser.h"
#include "builtin.h"
#include "path.h"

#define BLACK_BLUE "\033[40;34m"
#define BLUE_BLACK "\033[44;30m"
#define GREEN_BLACK "\033[42;30m"
#define BLACK_GREEN "\033[40;32m"
#define GREEN_GREEN "\033[42;32m"
#define RESET "\033[0m"

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void prompt(int maxn)
{
    char *path = getcwd(NULL, 0);
    if (path == NULL)
    {
        error_message();
        return;
    }
    int slahe = 0;
    int len = 0;
    int rem = 0;
    int j = 0;
    for (int i = 0; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
            slahe++;
        len += 1;
    }
    rem = max(0, slahe - maxn);
    char prompt[len + 1];
    for (int i = 0; path[i] != '\0'; ++i)
    {
        if (rem == 0)
        {
            prompt[j] = path[i];
            j++;
        }
        if (path[i] == '/' && rem > 0)
        {
            rem -= 1;
        }
    }
    printf("%s", BLUE_BLACK);
    printf(" 🕊 Gaza");
    printf("%s", BLACK_BLUE);
    printf("\uE0B0");
    printf("%s", GREEN_BLACK);
    prompt[j] = '\0';
    printf("\uE0B0%s \uE0B1\uE0B1\uE0B1", prompt);
    printf("%s", BLACK_GREEN);
    printf("\uE0B0");
    printf("%s", RESET);
    printf(" ");
    fflush(stdout);
    free(path);
}

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
        // printf("Wish> ");
        prompt(1);
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
        free(line);
        line = NULL;
    }
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