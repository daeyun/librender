#!/bin/bash
set -e

OPTS=$@
OPTS=${OPTS//--gen/}
OPTS=${OPTS//--cmake/}
OPTS=${OPTS//--make/}
OPTS=${OPTS//--test/}
OPTS=${OPTS//--lldb/}
OPTS=${OPTS//--debug/}
OPTS=${OPTS//--run/}
OPTS=${OPTS//--clean/}

function yellow {
tput setaf 3 && echo "$1" && tput sgr0
}
function green {
tput setaf 2 && echo "$1" && tput sgr0
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
yellow "cd ${DIR}/.."
cd ${DIR}/..

if [[ $@ =~ (--clean) ]]; then
yellow "safe-rm -rf build bin build-dev bin-dev"
safe-rm -rf build bin build-dev bin-dev
fi

CMAKE_ARGS="-H. "
MAKE_ARGS=""

if [[ $@ =~ (--test|--debug) ]]; then
CMAKE_ARGS+="-Bbuild-dev "
MAKE_ARGS+="-Cbuild-dev"
BIN_DIR="bin-dev"
else
CMAKE_ARGS+="-Bbuild "
MAKE_ARGS+="-Cbuild"
BIN_DIR="bin"
fi

DID_BUILD=false

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
DID_BUILD=true
fi

if [[ $@ =~ (--make) ]]; then
yellow "make ${MAKE_ARGS}"
make ${MAKE_ARGS}
DID_BUILD=true
fi


if [[ $@ =~ (--test) ]]; then
yellow "make ${MAKE_ARGS} test"
make ${MAKE_ARGS} test
DID_BUILD=true
fi

if ${DID_BUILD}; then
green "Build successful"
fi

if [[ $@ =~ (--lldb) ]]; then
yellow "cd ${BIN_DIR}"
cd ${BIN_DIR}
yellow "lldb -- ./scry ${OPTS}"
lldb -- ./scry ${OPTS}

elif [[ $@ =~ (--run) ]]; then
yellow "cd ${BIN_DIR}"
cd ${BIN_DIR}
yellow "./scry ${OPTS}"
./scry ${OPTS}
fi
