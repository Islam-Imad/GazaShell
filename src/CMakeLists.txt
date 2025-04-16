add_library(
    shell_lib
    "symbols.c"
    "scanner.c"
    "command.c"
    "builtin.c"
    "pipe.c"
    "conditional_cmd.c"
    "list.c"
    "parser.c"
)

target_include_directories(shell_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)