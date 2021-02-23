
rmdir /S /Q __build_WIN64
mkdir __build_WIN64
pushd __build_WIN64
cmake -A x64 -DCMAKE_BUILD_TYPE=debug ..
cmake --build . --config debug
popd

rem pause