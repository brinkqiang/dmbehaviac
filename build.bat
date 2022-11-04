
rmdir /S /Q __build_win64
mkdir __build_win64
pushd __build_win64
cmake -A x64 -DCMAKE_BUILD_TYPE=debug ..
cmake --build . --config debug -- /m:%NUMBER_OF_PROCESSORS%
popd

pause