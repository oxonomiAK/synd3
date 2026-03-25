#include "core/cli.h"
#include "config.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

static const struct option long_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {
        0,
        0,
        0,
        0,
    }};

// requires additional verification to prevent --h or --v
int parseArguments(int argc, char *argv[], AppConfig *config)
{
    int opt = 0;
    while ((opt = getopt_long(argc, argv, "hv", long_opts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            config->show_help = 1;
            break;

        case 'v':
            config->show_version = 1;
            break;

        default:
            fprintf(stderr, "%s: Unknown option\n", SYND3_NAME); // argv[optind-1]
            return 1;
        }
    }
    return 0;
}