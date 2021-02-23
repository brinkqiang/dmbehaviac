
rmdir /S /Q __build
mkdir __build
pushd __build
cmake -A x64 -DCMAKE_BUILD_TYPE=debug ..
cmake --build . --config debug
popd

rem pause