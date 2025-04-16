#include <stdio.h>
#include "parser.h"

int match(char *cmp_a, char *cmp_b)
{
    if (strcmp(cmp_a, cmp_b) == 0)
        return 1;
    return 0;
}

int is_io(char *token)
{
    if (match(token, "<") || match(token, ">") || match(token, "2>"))
        return 1;
    return 0;
}

int is_pipeline(char *token)
{
    if (match(token, "|"))
        return 1;
    return 0;
}

int is_conditional_cmd(char *token)
{
    if (match(token, "&&") || match(token, "||"))
        return 1;
    return 0;
}

int is_separator(char *token)
{
    if (match(token, "&") || match(token, ";"))
        return 1;
    return 0;
}

int is_special(char *token)
{
    if (is_separator(token) || is_io(token) || is_pipeline(token) || is_conditional_cmd(token))
    {
        return 1;
    }
    return 0;
}

struct list *parser(struct tokens *t)
{
    struct list *l = (struct list *)malloc(sizeof(struct list));
    if (l == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    init_list(l);
    int idx = 0;
    insert_ccmd(l, parse_conditional_cmd(t, &idx));
    while (idx < t->count && is_separator(t->tk[idx]) == 1)
    {
        if (match(t->tk[idx], "&"))
        {
            l->ccmds[l->ccmdno - 1]->background = 1;
        }
        idx += 1;
        insert_ccmd(l, parse_conditional_cmd(t, &idx));
    }
    return l;
}

struct conditional_cmd *parse_conditional_cmd(struct tokens *t, int *idx)
{
    struct conditional_cmd *cc = (struct conditional_cmd *)malloc(sizeof(struct conditional_cmd));
    if (cc == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    init_ccmd(cc);
    insert_pipeline(cc, parse_pipeline(t, idx));
    while (*idx < t->count && is_conditional_cmd(t->tk[*idx]) == 1)
    {
        if (match(t->tk[*idx], "||"))
        {
            set_state(cc->pipes[cc->pipno - 1], 1);
        }
        else
        {
            set_state(cc->pipes[cc->pipno - 1], 0);
        }
        *(idx) += 1;
        struct pipeline *p = parse_pipeline(t, idx);
        insert_pipeline(cc, p);
    }
    return cc;
}

struct pipeline *parse_pipeline(struct tokens *t, int *idx)
{
    struct pipeline *p = (struct pipeline *)malloc(sizeof(struct pipeline));
    if (p == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    init_pipline(p);
    insert_command(p, parse_command(t, idx));
    while (*idx < t->count && is_pipeline(t->tk[*idx]) == 1)
    {
        *(idx) += 1;
        insert_command(p, parse_command(t, idx));
    }
    return p;
}

struct command *parse_command(struct tokens *t, int *idx)
{
    struct command *cmd = (struct command *)malloc(sizeof(struct command));
    if (cmd == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return NULL;
    }
    init_command(cmd);
    parse_io(t, idx, cmd);
    while (*idx < t->count && is_special(t->tk[*idx]) == 0)
    {
        insert_arg(cmd, t->tk[*idx]);
        *(idx) += 1;
    }
    parse_io(t, idx, cmd);
    return cmd;
}

void parse_io(struct tokens *t, int *idx, struct command *cmd)
{
    if (*idx < t->count && is_io(t->tk[*idx]))
    {
        if (match(t->tk[*idx], ">"))
        {
            cmd->output = t->tk[*idx + 1];
        }
        else if (match(t->tk[*idx], "<"))
        {
            cmd->input = t->tk[*idx + 1];
        }
        else
        {
            cmd->error = t->tk[*idx + 1];
        }
        *(idx) += 2;
    }
}