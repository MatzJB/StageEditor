cmake_minimum_required(VERSION 3.16)
project(StageEditor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CONFIGURATION_TYPES)
    set(CTEST_CONFIGURATION_TYPE "Debug")
endif()

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find Vulkan
find_package(Vulkan REQUIRED)
find_package(glm CONFIG REQUIRED)

# ====================
# Fetch SDL3
# ====================
include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.8
)
FetchContent_MakeAvailable(SDL3)

# ====================
# ImGui Sources
# ====================
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl3.cpp
    external/imgui/backends/imgui_impl_vulkan.cpp
)

# ====================
# Source and Header Files (excluding main)
# ====================
file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

file(GLOB_RECURSE LIB_HEADERS
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/include/*.hpp
)

# ====================
# Create Static Library
# ====================
add_library(StageEditorLib STATIC ${LIB_SOURCES} ${LIB_HEADERS} ${IMGUI_SOURCES})

target_include_directories(StageEditorLib PRIVATE
    external/imgui
    external/imgui/backends
    ${CMAKE_SOURCE_DIR}/include
    ${SDL3_SOURCE_DIR}/include  # for FetchContent
)

target_link_libraries(StageEditorLib
    PUBLIC
    Vulkan::Vulkan
    SDL3::SDL3
    glm::glm
)

# ====================
# Visual Studio Filters
# ====================
# Group headers, sources, and ImGui files
source_group("Header Files" FILES ${LIB_HEADERS})
source_group("Source Files" FILES ${LIB_SOURCES})
source_group("ImGui" FILES ${IMGUI_SOURCES})

# Optional: preserve folder structure for headers
foreach(_header ${LIB_HEADERS})
    get_filename_component(_dir "${_header}" PATH)
    file(RELATIVE_PATH _rel_dir "${CMAKE_SOURCE_DIR}" "${_dir}")
    source_group("${_rel_dir}" FILES "${_header}")
endforeach()

foreach(_src ${LIB_SOURCES})
    get_filename_component(_dir "${_src}" PATH)
    file(RELATIVE_PATH _rel_dir "${CMAKE_SOURCE_DIR}" "${_dir}")
    source_group("${_rel_dir}" FILES "${_src}")
endforeach()

# ====================
# Executable
# ====================
add_executable(StageEditorMain src/main.cpp)


if (WIN32)
    add_custom_command(TARGET StageEditorMain POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:StageEditorMain>
    )
endif()

target_include_directories(StageEditorMain PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    external/imgui
    external/imgui/backends
    ${SDL3_SOURCE_DIR}/include
)

target_link_libraries(StageEditorMain PRIVATE
    StageEditorLib
)

# ====================
# Shader Copying
# ====================
set(SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/src/shaders")
set(SHADER_DST_DIR "${CMAKE_BINARY_DIR}/shaders")

file(GLOB SHADER_FILES "${SHADER_SRC_DIR}/*.spv")

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_FILENAME ${SHADER_FILE} NAME)
    add_custom_command(
        TARGET StageEditorMain POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_DST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SHADER_FILE}"
                "${SHADER_DST_DIR}/${SHADER_FILENAME}"
        COMMENT "Copying ${SHADER_FILENAME} to build/shaders"
    )
endforeach()

# ====================
# GoogleTest
# ====================
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 6910c9d  # 1.16.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB UNIT_TEST_SOURCES tests/*.cpp)
add_executable(StageEditorTests ${UNIT_TEST_SOURCES})

target_link_libraries(StageEditorTests PRIVATE
    gtest_main
    StageEditorLib
)

add_test(NAME unit_tests COMMAND StageEditorTests)
