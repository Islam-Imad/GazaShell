#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "command.h"
#include "pipe.h"

void init_pipline(struct pipeline *p)
{
    p->comno = 0;
    p->state = -1;
    for (int i = 0; i < MAX_PIPE; ++i)
        p->cmd[i] = NULL;
}

void set_state(struct pipeline *p, int sstate)
{
    p->state = sstate;
}

void insert_command(struct pipeline *p, struct command *cmd)
{
    if (p->comno < MAX_PIPE)
    {
        p->cmd[p->comno] = cmd;
        p->comno += 1;
    }
    else
    {
        fprintf(stderr, "Too many pipes");
        exit(EXIT_FAILURE);
    }
}

void free_pipeline(struct pipeline *p)
{
    for (int i = 0; i < p->comno; ++i)
    {
        free_command(p->cmd[i]);
        free(p->cmd[i]);
        p->cmd[i] = NULL;
    }
}

void print_pipeline(struct pipeline *p)
{
    printf("Pipeline : %d :\n", p->comno);
    for (int i = 0; i < p->comno; ++i)
    {
        print_command(p->cmd[i]);
        if (i != p->comno - 1)
            printf(" | ");
    }
    printf("\n");
}

int execute_pipeline(struct pipeline *p)
{
    int gst = 0;
    if (p->comno == 0)
        return gst;
    int tmpin = dup(STDIN_FILENO);
    int tmpout = dup(STDOUT_FILENO);
    int fdin = dup(STDIN_FILENO);
    if (p->cmd[0]->input != NULL)
    {
        fdin = open(p->cmd[0]->input, O_RDONLY);
        if (fdin < 0)
        {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else
        {
            dup2(fdin, STDIN_FILENO);
        }
    }

    for (int i = 0; i < p->comno; ++i)
    {
        int fd2[2];
        if (pipe(fd2) == -1)
        {
            perror("exit :(");
            exit(EXIT_FAILURE);
        }
        if (i + 1 == p->comno)
        {
            if (p->cmd[i]->output != NULL)
            {
                close(fd2[1]);
                fd2[1] = open(p->cmd[i]->output, O_WRONLY | O_CREAT, 0644);
                if (fd2[1] == -1)
                {
                    perror("opening a file to write");
                    exit(EXIT_FAILURE);
                }
                dup2(fd2[1], STDOUT_FILENO);
            }
            else
            {
                dup2(tmpout, STDOUT_FILENO);
            }
        }
        else
        {
            dup2(fd2[1], STDOUT_FILENO);
        }
        // don't expect me to write again (close output - write end)
        int st = execute_command(p->cmd[i]);
        if (st != 0)
        {
            gst = -1;
            break;
        }
        close(fd2[1]);
        dup2(fd2[0], STDIN_FILENO);
        fdin = fd2[0];
    }
    close(fdin);
    dup2(tmpin, STDIN_FILENO);
    dup2(tmpout, STDOUT_FILENO);
    close(tmpin);
    close(tmpout);
    return gst;
}