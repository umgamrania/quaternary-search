#!/bin/bash

# seq (start) (step) (end)

for a in `seq 50000 50000 3000000`
    do
        clear
        echo "Executing `expr $a / 50000`"
        ./search $a 10
done
