#!/bin/sh
set -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd ${DIR}/..

cmake -H. -Bbuild
make -Cbuild
echo "Build successful"
