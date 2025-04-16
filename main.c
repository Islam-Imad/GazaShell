#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "symbols.h"
#include "scanner.h"
#include "command.h"
#include "pipe.h"
#include "conditional_cmd.h"

char *read_line()
{
    char *buffer = NULL;
    size_t len = 0;
    ssize_t read;
    read = getline(&buffer, &len, stdin);
    if (read == -1)
    {
        perror("getline");
        return NULL;
    }
    return buffer;
}

void test_1()
{
    struct command cmd;
    init_command(&cmd);
    insert_arg(&cmd, "cat");
    insert_arg(&cmd, "../input.txt");
    struct command cmd2;
    init_command(&cmd2);
    insert_arg(&cmd2, "grep");
    insert_arg(&cmd2, "a");
    struct command cmd3;
    init_command(&cmd3);
    insert_arg(&cmd3, "grep");
    insert_arg(&cmd3, "c");
    struct command cmd4;
    init_command(&cmd4);
    insert_arg(&cmd4, "grep");
    insert_arg(&cmd4, "d");
    set_output(&cmd4, "../output.txt");
    struct pipeline p;
    init_pipline(&p);
    insert_command(&p, &cmd);
    insert_command(&p, &cmd2);
    insert_command(&p, &cmd3);
    insert_command(&p, &cmd4);
    set_state(&p, 1);
    // just continue if the last command fails
    struct conditional_cmd cc;
    init_ccmd(&cc);
    insert_pipeline(&cc, &p);
    set_background(&cc, 1);
    print_conditional_cmd(&cc);
    execute_conditional_cmd(&cc);
    free_conditional_cmd(&cc);
}

void test_2()
{
    struct command cmd;
    init_command(&cmd);
    insert_arg(&cmd, "cat");
    insert_arg(&cmd, "../input.txt");
    struct command cmd2;
    init_command(&cmd2);
    insert_arg(&cmd2, "ls");
    struct pipeline p;
    init_pipline(&p);
    insert_command(&p, &cmd);
    set_state(&p, 1);
    struct pipeline p2;
    init_pipline(&p2);
    insert_command(&p2, &cmd2);
    set_state(&p2, 0);
    // just continue if the last command fails
    struct conditional_cmd cc;
    init_ccmd(&cc);
    insert_pipeline(&cc, &p);
    insert_pipeline(&cc, &p2);
    set_background(&cc, 1);
    print_conditional_cmd(&cc);
    execute_conditional_cmd(&cc);
    free_conditional_cmd(&cc);
}

int main(int argc, char *argv[])
{
    test_2();
}