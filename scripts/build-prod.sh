#!/bin/bash
set -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd ${DIR}/..

safe-rm -rf build bin build-dev bin-dev

python3 src/shaders/glsl/gen_h.py
cmake -H. -Bbuild -Dtest=OFF -DDEBUG=OFF
make -Cbuild
echo "Build successful"
