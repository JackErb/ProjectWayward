rm -rf build/
mkdir build
cd build

CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=Debug"

if [ "$1" == "xcode" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -G Xcode"
fi

cmake ../src $CMAKE_FLAGS
