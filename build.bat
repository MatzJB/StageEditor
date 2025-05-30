@echo off

set PATH=C:\Qt\6.9.0\msvc2022_64\bin;%PATH%

;;"c:\git\vcpkg\vcpkg.exe" install --triplet x64-windows
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/git/vcpkg/scripts/buildsystems/vcpkg.cmake



if not exist build mkdir build


cd build

cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.9.0/msvc2022_64" -G "Visual Studio 17 2022"


cmake --build . --config Debug


