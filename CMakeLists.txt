cmake_minimum_required(VERSION 3.11)

# Require modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WraithBound)

# ### Setup Directories ####
# Main output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Escaped paths for Inno Setup
string(REPLACE "\\" "/" CMAKE_RUNTIME_OUTPUT_DIRECTORY_ESCAPED ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# ### Find OpenGL ####
find_package(OpenGL REQUIRED)

if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL libraries not found. Please install OpenGL development packages.")
endif()

# ### Find OpenAL ####
find_package(OpenAL REQUIRED)

if(NOT OpenAL_FOUND)
    message(FATAL_ERROR "OpenAL libraries not found. Please install OpenAL development packages.")
endif()

# ### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# ### Create Libraries ####
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

# ### Copy Resources ####
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)

# ### Game ####
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(WraithBound ${SOURCES})
target_include_directories(WraithBound SYSTEM PRIVATE ${SFML_INCS})

# Link the necessary SFML and OpenAL libraries
target_link_libraries(WraithBound
    lib_tile_level_loader
    lib_maths
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    OpenAL::OpenAL
)

# # Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/")
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

set_target_properties(WraithBound
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
add_dependencies(WraithBound copy_resources)

# ### Inno Setup Configuration ####
# Configure the Inno Setup file with escaped paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ### Inno Setup Configuration ####
# Configure the Inno Setup file with escaped paths
configure_file(${CMAKE_SOURCE_DIR}/setup.iss.in ${CMAKE_BINARY_DIR}/setup.iss @ONLY)

# Create an installer target that runs the Inno Setup compiler
add_custom_target(installer
    COMMAND "C:/Program Files (x86)/Inno Setup 6/ISCC.exe" ${CMAKE_BINARY_DIR}/setup.iss
    DEPENDS WraithBound copy_resources
    COMMENT "Generating Inno Setup installer"
)
