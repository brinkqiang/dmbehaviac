
rmdir /S /Q __build_win64
mkdir __build_win64
pushd __build_win64
cmake -A x64 -DCMAKE_BUILD_TYPE=relwithdebinfo ..
cmake --build . --config relwithdebinfo -- /m:%NUMBER_OF_PROCESSORS%
popd

pause