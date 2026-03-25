#ifndef CLI_H
#define CLI_H

typedef struct
{
    int show_help;
    int show_version;
} AppConfig;

int parseArguments(int argc, char *argv[], AppConfig *config);

#endif