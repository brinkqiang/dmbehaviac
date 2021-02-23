#!/bin/bash

# - install depends tools
# yum -y install git
# yum -y install gcc gcc-c++ autoconf libtool automake make
#

# pushd thirdparty/depends_path
# libtoolize && aclocal && autoheader && autoconf && automake --add-missing
# sh configure
# popd

rm -rf __build
mkdir -p __build
pushd __build

cmake -DCMAKE_BUILD_TYPE=debug ..
cmake --build . --config debug

popd
# popd

# echo continue && read -n 1
