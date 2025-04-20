#include <iostream>
#include "../include/ncurs.h"
#include "../include/func.h"

int main()
{

    std::cout << "Starting of main thread of glance..." << std::endl;

    int x = 5;

    std::cout << "do_test_mult from \'func.c\': " << do_test_mult(&x) << std::endl;

    init_ui();

    return 0;
}