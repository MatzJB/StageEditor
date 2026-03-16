@echo off

set PATH=C:\VulkanSDK\1.4.313.1\Bin;%PATH%

:: Optional: Set path to Vulkan SDK if not already in PATH
:: set VULKAN_SDK=C:\VulkanSDK\1.3.x.x\Bin
:: set PATH=%VULKAN_SDK%;%PATH%

glslc src\shaders\vertex.vert -o src\shaders\vertex.spv
glslc src\shaders\fragment.frag -o src\shaders\fragment.spv

::"c:\git\vcpkg\vcpkg.exe" install --triplet x64-windows
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/git/vcpkg/scripts/buildsystems/vcpkg.cmake

if not exist build mkdir build
cd build

::cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.9.0/msvc2022_64" -G "Visual Studio 17 2022"
cmake --build . --config Debug


endlocal