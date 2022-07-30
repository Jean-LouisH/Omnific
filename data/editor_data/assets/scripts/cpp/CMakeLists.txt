# Omnia native C++ scripting build

cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE omnia_cpp_native_assembly_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnia_cpp_native_assembly_headers "[a-zA-Z]*.hpp")


add_library(omnia_cpp_native_assembly SHARED
	  ${omnia_cpp_native_assembly_sources}
	  ${omnia_cpp_native_assembly_headers}
)

 add_definitions(/bigobj)

include_directories(
		.
		../../../../../src/engine
)

target_link_libraries (omnia_cpp_native_assembly

)

add_definitions(-D_OMNIA_CPP_NATIVE_ASSEMBLY_EXPORTS)

add_custom_command(TARGET omnia_cpp_native_assembly POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:omnia_cpp_native_assembly> ${PROJECT_BINARY_DIR}/src/main/omnia_cpp_native_assembly.dll
)