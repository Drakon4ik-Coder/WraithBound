cmake_minimum_required(VERSION 3.11)

# Require modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WraithBound)

# ### Setup Directories ####
# Main output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

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

# # Tile loader lib
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/")
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

# ### Copy Resources ####
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)

add_custom_target(copy_resources2 ALL COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/res
)

add_custom_target(copy_resources_release ALL COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/res
)

# Copy OpenAL32.dll to the output directory for different configurations
add_custom_target(copy_openal_dll ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    "${PROJECT_SOURCE_DIR}/external/OpenAL/OpenAL32.dll"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/OpenAL32.dll
)

# Copy OpenAL32.lib for linking
add_custom_target(copy_openal_lib ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    "${PROJECT_SOURCE_DIR}/external/OpenAL/OpenAL32.dll"
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OpenAL32.dll
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

set_target_properties(WraithBound
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)

# ### Installer ####
configure_file(${CMAKE_SOURCE_DIR}/setup.iss.in ${CMAKE_BINARY_DIR}/setup.iss @ONLY)

add_custom_target(installer
    COMMAND "C:/Program Files (x86)/Inno Setup 6/ISCC.exe" ${CMAKE_BINARY_DIR}/setup.iss
    DEPENDS WraithBound
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating installer using Inno Setup"
)

# Ensure the copy_openal_dll and copy_openal_lib targets are built before the installer
add_dependencies(installer copy_openal_dll copy_openal_lib)
