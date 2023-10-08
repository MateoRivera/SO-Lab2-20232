#! /bin/bash

if ! [[ -x wish ]]; then
    echo "wish executable does not exist"
    exit 1
fi
echo "Running tests..."
../tester/run-tests.sh -t 7 $*


