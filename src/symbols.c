#include <stdio.h>
#include "symbols.h"

int get_symbol(char *str){
    if (str[0] == '&' && str[1] == '\0')
        return parallel;
    else if (str[0] == ';' && str[1] == '\0')
        return then;
    else if (str[0] == '|' && str[1] == '\0')
        return pipe_;
    else if (str[0] == '&' && str[1] == '&' && str[2] == '\0')
        return andf;
    else if (str[0] == '|' && str[1] == '|' && str[2] == '\0')
        return orf;
    else if (str[0] == '<' && str[1] == '\0')
        return less;
    else if (str[0] == '>' && str[1] == '\0')
        return great;
    else if (str[0] == '<' && str[1] == '<' && str[2] == '\0')
        return dless;
    else if (str[0] == '>' && str[1] == '>' && str[2] == '\0')
        return dgreat;
    else if (str[0] == '<' && str[1] == '&' && str[2] == '\0')
        return lessand;
    else if (str[0] == '>' && str[1] == '&' && str[2] == '\0')
        return greatand;
    else
        return -1; // Not a symbol
}