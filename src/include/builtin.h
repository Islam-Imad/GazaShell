#include <stdio.h>
#include "command.h"

#ifndef BUILTIN_H
#define BUILTIN_H

void error_message();

int is_file(char *path);

char *get_pwd();

int execute_pwd(struct command *cmd);

int execute_cd(struct command *cmd);

int excute_exit(struct command *cmd);

int execute_help(struct command *cmd);

int execute_path(struct command *cmd, struct path *p);

int check_builtin(struct command *cmd);

int excute_builtin(struct command *cmd, struct path *p);

#endif // BUILTIN_H