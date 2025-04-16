#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SCANNER_H
#define SCANNER_H

#define MAX_TOKENS 256

struct tokens
{
    char *tk[MAX_TOKENS]; // Array of tokens
    int count;            // Number of tokens
};

void init_tokens(struct tokens *tokens);

void insert_token(struct tokens *tokens, char *token);

void print_tokens(struct tokens *tokens);

void free_tokens(struct tokens *tokens);

struct tokens *scanner(char *source);

#endif // SCANNER_H