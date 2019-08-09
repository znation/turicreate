#!/bin/bash
# has to be run from root of the repo
set -x
set -e

if [[ -z $VIRTUALENV ]]; then
  VIRTUALENV=virtualenv
fi

BIN_DIR="bin"
EXE_EXTENSION=""
if [[ $OSTYPE == "msys" ]]; then
  BIN_DIR="Scripts"
  EXE_EXTENSION=".exe"
fi

$VIRTUALENV `pwd`/deps/env
source deps/env/$BIN_DIR/activate

PYTHON="${PWD}/deps/env/$BIN_DIR/python$EXE_EXTENSION"
PIP="${PYTHON} -m pip"

PYTHON_MAJOR_VERSION=$(${PYTHON} -c 'import sys; print(sys.version_info.major)')
PYTHON_MINOR_VERSION=$(${PYTHON} -c 'import sys; print(sys.version_info.minor)')
PYTHON_VERSION="python${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}"

# TODO - not sure why 'm' is necessary here (and not in 2.7)
# note that PYTHON_VERSION includes the word "python", like "python2.7" or "python3.6"
PYTHON_FULL_NAME=${PYTHON_VERSION}m
if [[ "${PYTHON_VERSION}" == "python2.7" ]]; then
  PYTHON_FULL_NAME=python2.7
fi


function linux_patch_sigfpe_handler {
  if [[ $OSTYPE == linux* ]]; then
    targfile=deps/local/include/pyfpe.h
    if [[ -f $targfile ]]; then
      echo "#undef WANT_SIGFPE_HANDLER" | cat - $targfile > tmp
      mv -f tmp $targfile
    fi
  fi
}

$PIP install --upgrade "pip>=8.1"
$PIP install -r scripts/requirements.txt

mkdir -p deps/local/lib
mkdir -p deps/local/include

pushd deps/local/include
for f in `ls -d ../../env/include/$PYTHON_FULL_NAME/*`; do
  ln -Ffs $f
done
popd

if [[ $OSTYPE != "msys" ]]; then
  mkdir -p deps/local/bin
  pushd deps/local/bin
  for f in `ls ../../env/$BIN_DIR`; do
    ln -Ffs $f
  done
  popd
fi

linux_patch_sigfpe_handler

