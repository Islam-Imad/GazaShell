#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

void init_tokens(struct tokens *tokens)
{
    tokens->count = 0;
    for (int i = 0; i < MAX_TOKENS; ++i)
    {
        tokens->tk[i] = NULL;
    }
}

void insert_token(struct tokens *tokens, char *token)
{
    if (tokens->count < MAX_TOKENS)
    {
        tokens->tk[tokens->count] = strdup(token);
        tokens->count++;
    }
}

void print_tokens(struct tokens *tokens)
{
    for (int i = 0; i < tokens->count; ++i)
    {
        printf("Token %d: %s\n", i, tokens->tk[i]);
    }
}

void free_tokens(struct tokens *tokens)
{
    for (int i = 0; i < tokens->count; ++i)
    {
        free(tokens->tk[i]);
    }
    free(tokens);
}

struct tokens *scanner(char *source)
{
    struct tokens *tks = (struct tokens *)malloc(sizeof(struct tokens));
    init_tokens(tks);
    const char delimiters[] = " \t\n";
    char *soruce_copy = strdup(source);
    char *token = strsep(&soruce_copy, delimiters);
    while (token != NULL)
    {
        if (strlen(token) > 0)
        {
            insert_token(tks, token);
        }
        token = strsep(&soruce_copy, delimiters);
    }
    free(soruce_copy);
    return tks;
}