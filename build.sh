#!/bin/sh
set -e
cmake -H. -Bbuild
make -Cbuild
echo "Build successful"
