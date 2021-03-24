#!/usr/bin/env bash

BUILD_DIR=_build

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake .. && make
cd -
