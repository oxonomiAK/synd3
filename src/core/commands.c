#include "core/commands.h"
#include "config.h"
#include <stdio.h>

// make correct version info
void print_help(void)
{
    printf(
        "%s %s\n"
        "(C) 2025-2026 %s dev team.\n"
        "Released under the MIT.\n\n"
        "Usage: synd3 [OPTIONS]\n"
        "   -h, --help          Print this help screen\n"
        "   -v, --version       Print version info\n",
        SYND3_NAME, SYND3_VERSION, SYND3_NAME);
}

void print_version(void)
{
    printf("%s %s\n", SYND3_NAME, SYND3_VERSION);
}
