# The Grammar

- `Programm`          : linebreak complete_commands linebreak | linebreak

- `complete_commands` : complet_commands newline_list complete_command | complete_command

- `complete_command`  : list seperator_op | list

- `list`              : list seperator_op and_or | and_or

- `and_or`            : pipeline | and_or AND_IF linebreak pipeline | and_or OR_IF linebreak pipeline

- `pipeline`          : command | pipeline '|' linebreak command

- `command`           : simple_command | compound_command

- `simple_command`    : command_name | command_name argument_list

- `compound_command`  : simple_command | io simple_command io

- `command_name`      : WORD

- `argument_list`     : argument_list argument | argument

- `argument`          : WORD

- `linebreak`         : newline_list | empty

- `newline_list`      : newline_list newline | newline

- `empty`             : ""

- `seperator_op`      : & | ;
