#!/bin/bash
set -e

function yellow {
tput setaf 3 && echo "$1" && tput sgr0
}
function green {
tput setaf 2 && echo "$1" && tput sgr0
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
yellow ${DIR}/..
cd ${DIR}/..

OPTS=$@
OPTS=${OPTS//--gen/}
OPTS=${OPTS//--cmake/}
OPTS=${OPTS//--make/}
OPTS=${OPTS//--test/}
OPTS=${OPTS//--devel/}
OPTS=${OPTS//--debug/}
OPTS=${OPTS//--run/}

CMAKE_ARGS="-H. "
MAKE_ARGS=""

if [[ $@ =~ (--devel) ]]; then
CMAKE_ARGS+="-Bbuild-dev "
MAKE_ARGS+="-Cbuild-dev"
BIN_DIR="bin-dev"
else
CMAKE_ARGS+="-Bbuild "
MAKE_ARGS+="-Cbuild"
BIN_DIR="bin"
fi

if [[ $@ =~ (--test) ]]; then
CMAKE_ARGS+="-DTEST=ON "
else
CMAKE_ARGS+="-DTEST=OFF "
fi

if [[ $@ =~ (--debug) ]]; then
CMAKE_ARGS+="-DDEBUG=ON "
else
CMAKE_ARGS+="-DDEBUG=OFF "
fi

if [[ $@ =~ (--gen) ]]; then
yellow "python3 src/shaders/glsl/gen_h.py"
python3 src/shaders/glsl/gen_h.py
fi

if [[ $@ =~ (--cmake) ]]; then
yellow "cmake ${CMAKE_ARGS}"
cmake ${CMAKE_ARGS}
fi

if [[ $@ =~ (--make) ]]; then
yellow "make ${MAKE_ARGS}"
make ${MAKE_ARGS}
fi


if [[ $@ =~ (--test) ]]; then
yellow "make ${MAKE_ARGS} test"
make ${MAKE_ARGS} test
fi

green "Build successful"

if [[ $@ =~ (--run) ]]; then
yellow "cd ${BIN_DIR}"
cd ${BIN_DIR}
yellow "./scry ${OPTS}"
./scry ${OPTS}
fi

