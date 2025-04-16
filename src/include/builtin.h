#include <stdio.h>
#include "command.h"

#ifndef BUILTIN_H
#define BUILTIN_H

void error_message();

int execute_pwd();

int execute_cd(char *path);

int excute_exit();

int execute_help();

int check_builtin(struct command *cmd);

int excute_builtin(struct command *cmd);

#endif // BUILTIN_H