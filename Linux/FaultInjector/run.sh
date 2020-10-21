#!/bin/bash

output=$(g++ FaultInjector.cpp -o FIng 2>&1)
if [[ $? != 0 ]]; then
    # There was an error, display the error in $output
    echo -e "Error:\n$output"
else
    # Compilation successfull
./FIng
fi
