#set(CMAKE_FIND_DEBUG_MODE 1)

# Set the minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(StageEditor VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CONFIGURATION_TYPES)
    set(CTEST_CONFIGURATION_TYPE "Debug")
endif()


find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)


file(COPY src/vertex.glsl src/fragment.glsl DESTINATION ${CMAKE_BINARY_DIR})


# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect source files EXCLUDING main.cpp
file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Remove src/main.cpp from LIB_SOURCES
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")




find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)



# Create the library
add_library(StageEditorLib ${LIB_SOURCES})

# Add the main executable (the application)
add_executable(StageEditorMain src/main.cpp)
# target_link_libraries(StageEditorMain PRIVATE StageEditorLib)



target_link_libraries(StageEditorMain
    PRIVATE
        StageEditorLib
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
)



# Set build type
set(CMAKE_BUILD_TYPE Debug)

# Add Google Test using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        6910c9d  # 1.16.0
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE) # Disable GMock
FetchContent_MakeAvailable(googletest)
set(CTEST_CONFIGURATION_TYPE Debug)
enable_testing()


file(GLOB UNIT_TEST_SOURCES tests/*.cpp)

add_executable(StageEditorTests ${UNIT_TEST_SOURCES})
target_link_libraries(StageEditorTests gtest_main StageEditorLib)


add_test(NAME unit_tests COMMAND StageEditorTests)





