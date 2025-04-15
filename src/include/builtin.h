#include <stdio.h>
#include "command.h"

const char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "path"
};

const int count_builtin = 4;

void execute_cd(char *path);

void excute_exit();

void execute_help();

int is_builtin(struct command *cmd);

void excute_builtin(struct command *cmd);