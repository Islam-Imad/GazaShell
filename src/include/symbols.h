#include <stdio.h>

enum symbols
{
    parallel, /* & */
    then,     /* ; */
    pipe_,    /* | */
    andf,     /* && */
    orf,      /* || */
    less,     /* < */
    great,    /* > */
    dless,    /* << */
    dgreat,   /* >> */
    lessand,  /* <& */
    greatand, /* >& */
    word,     /* word */
    end,      /* end of input */
};

const char *seperator[] = {"&", ";"};

int get_symbol(char *str);