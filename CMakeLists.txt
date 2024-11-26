# CMakeLists.txt
cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WraithBound)

#### Setup Directories ####
# Main output directory
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Output all DLLs from all libs into main build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Find OpenGL ####
find_package(OpenGL REQUIRED)
if (NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL libraries not found. Please install OpenGL development packages.")
endif()

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/")
target_link_libraries(lib_tile_level_loader sfml-graphics OpenGL::GL)

# Maths lib
add_library(lib_math INTERFACE)
target_sources(lib_math INTERFACE "${CMAKE_SOURCE_DIR}/lib_math/Math.h")
target_include_directories(lib_math INTERFACE "${CMAKE_SOURCE_DIR}/lib_math" SYSTEM INTERFACE ${SFML_INCS})

#### Main Executable ####
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(WraithBound ${SOURCES})
target_include_directories(WraithBound PRIVATE ${SFML_INCS})
target_link_libraries(WraithBound lib_tile_level_loader lib_math sfml-graphics OpenGL::GL)

# Debugger settings (for Visual Studio or other IDEs)
set_target_properties(WraithBound 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)

# Resource copying
add_dependencies(WraithBound copy_resources)
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)
