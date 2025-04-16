#include <stdio.h>
#include "command.h"

const char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "path",
    "pwd"
};

const int count_builtin = 4;

void error_message();

int execute_pwd();

int execute_cd(char *path);

int excute_exit();

int execute_help();

int is_builtin(struct command *cmd);

int excute_builtin(struct command *cmd);