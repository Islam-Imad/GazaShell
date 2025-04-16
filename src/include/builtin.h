#include <stdio.h>
#include "command.h"

const char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "path"
};

const int count_builtin = 4;

int execute_cd(char *path);

int excute_exit();

int execute_help();

int is_builtin(struct command *cmd);

int excute_builtin(struct command *cmd);