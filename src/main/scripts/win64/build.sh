#!/usr/bin/env bash

cmake . \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX="../../../../target/classes/win64" \
-DCMAKE_TOOLCHAIN_FILE=i686-w64-mingw32.static-cmake

cd curl/win64/lib
ls -l
cd ../../..

make
r1=$?

exit ${r1}
