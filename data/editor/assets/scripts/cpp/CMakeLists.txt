# Esi native C++ scripting build

cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE esi_cpp_native_assembly_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE esi_cpp_native_assembly_headers "[a-zA-Z]*.hpp")


add_library(esi_cpp_native_assembly SHARED
	  ${esi_cpp_native_assembly_sources}
	  ${esi_cpp_native_assembly_headers}
)

 add_definitions(/bigobj)

include_directories(
		.
		../../../../../src/engine
)

target_link_libraries (esi_cpp_native_assembly

)

add_definitions(-D_ESI_CPP_NATIVE_ASSEMBLY_EXPORTS)

add_custom_command(TARGET esi_cpp_native_assembly POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:esi_cpp_native_assembly> ${PROJECT_BINARY_DIR}/src/main/esi_cpp_native_assembly.dll
)