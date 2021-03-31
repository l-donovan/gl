#!/usr/bin/env bash

BUILD_DIR=_build
DIST_DIR=dist

RC=1

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake .. && make && RC=0
cd -

cp $BUILD_DIR/engine/src/engine $DIST_DIR/engine

exit $RC
