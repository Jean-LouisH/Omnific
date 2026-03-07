# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include sub-projects.
add_subdirectory("src/engine")
add_subdirectory("src/applications/editor")
add_subdirectory("src/applications/demo")
add_subdirectory("src/launcher")
add_subdirectory("tests")
add_subdirectory("thirdparty")

message (STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message (STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# Create a symbolic link for clangd in the project root
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CONF_FILE "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    set(ROOT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json")

    if(NOT EXISTS ${ROOT_FILE})
        execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CONF_FILE} ${ROOT_FILE})
    endif()
endif()
