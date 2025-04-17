add_library(
    shell_lib
    "scanner.c"
    "command.c"
    "builtin.c"
    "pipe.c"
    "conditional_cmd.c"
    "list.c"
    "parser.c"
    "path.c"
)

target_include_directories(shell_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)