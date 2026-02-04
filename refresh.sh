#!/bin/bash
make clean
make
find src -name '*.o' -delete
./synd3

#!/bin/bash