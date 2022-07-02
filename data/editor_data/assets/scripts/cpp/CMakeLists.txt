# Esi native C++ scripting build

cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE omnific_cpp_native_assembly_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnific_cpp_native_assembly_headers "[a-zA-Z]*.hpp")


add_library(omnific_cpp_native_assembly SHARED
	  ${omnific_cpp_native_assembly_sources}
	  ${omnific_cpp_native_assembly_headers}
)

 add_definitions(/bigobj)

include_directories(
		.
		../../../../../src/engine
)

target_link_libraries (omnific_cpp_native_assembly

)

add_definitions(-D_OMNIFIC_CPP_NATIVE_ASSEMBLY_EXPORTS)

add_custom_command(TARGET omnific_cpp_native_assembly POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:omnific_cpp_native_assembly> ${PROJECT_BINARY_DIR}/src/main/omnific_cpp_native_assembly.dll
)