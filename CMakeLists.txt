# CMakeLists.txt
cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GameEng)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES} "src/Entity.cpp" "src/Entity.h" "src/Player.h" "src/Player.cpp")
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader sfml-graphics)

# Maths lib
add_library(lib_math INTERFACE)
target_sources(lib_math INTERFACE "${CMAKE_SOURCE_DIR}/lib_math/Math.h")
target_include_directories(lib_math INTERFACE "${CMAKE_SOURCE_DIR}/lib_math" SYSTEM INTERFACE ${SFML_INCS})

#### Practical 1 ####
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(GameEng ${SOURCES} "src/Entity.cpp" "src/Entity.h" "src/Player.h" "src/Player.cpp")
target_include_directories(GameEng PRIVATE ${SFML_INCS})
target_link_libraries(GameEng lib_tile_level_loader lib_math sfml-graphics)

set_target_properties(GameEng 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
add_dependencies(GameEng copy_resources)

add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)


