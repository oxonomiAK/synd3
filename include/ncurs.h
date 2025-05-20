#ifndef NCURS_H
#define NCURS_H

#include <vector>
#include "core/process.h"

class NcursesUI
{
public:
    void init();
    void render(const std::vector<Process> &processes);
    void shutdown();
};

#endif