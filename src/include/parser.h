#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "list.h"
#include "scanner.h"
#include "command.h"
#include "pipe.h"
#include "conditional_cmd.h"

int match(char *cmp_a, char *cmp_b);

int is_io(char *token);

int is_pipeline(char *token);

int is_conditional_cmd(char *token);

int is_separator(char *token);

int is_special(char *token);

struct list *parser(struct tokens *t);

struct conditional_cmd *parse_conditional_cmd(struct tokens *t, int *idx);

struct pipeline *parse_pipeline(struct tokens *t, int *idx);

struct command *parse_command(struct tokens *t, int *idx);

void parse_io(struct tokens *t, int *idx,struct command *cmd);