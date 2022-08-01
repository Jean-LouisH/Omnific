# Omnia native C++ scripting build

cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE omnia_cpp_script_assembly_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnia_cpp_script_assembly_headers "[a-zA-Z]*.hpp")


add_library(omnia_cpp_script_assembly SHARED
	  ${omnia_cpp_script_assembly_sources}
	  ${omnia_cpp_script_assembly_headers}
)

 add_dependencies(omnia_cpp_script_assembly
	omnia_engine
)

 add_definitions(/bigobj)

include_directories(
		../../../../../thirdparty/SDL2-2.0.12/include
		../../../../../thirdparty/SDL2_mixer-2.0.4/include
		../../../../../thirdparty/SDL2_ttf-2.0.18/include
		../../../../../thirdparty/yaml-cpp-0.6.3/include
		../../../../../thirdparty/python37/include
		../../../../../thirdparty/pybind11-2.8.1/include
		../../../../../thirdparty/glad/include
		../../../../../thirdparty/glm-0.9.9.8/glm
		../../../../../thirdparty/tinygltf-2.5.0/include
		../../../../../thirdparty/zlib-1.2.11/include
		../../../../../thirdparty/stb_image_write-1.16/include
		../../../../../thirdparty/stb_image-2.27/include
		../../../../../thirdparty/freetype-2.11.1/include
		.
		../../../../../src/engine
)

target_link_libraries (omnia_cpp_script_assembly
	omnia_engine
)

add_definitions(-D_OMNIA_CPP_SCRIPT_ASSEMBLY_EXPORTS)

add_custom_command(TARGET omnia_cpp_script_assembly POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:omnia_cpp_script_assembly> ${PROJECT_BINARY_DIR}/src/main/omnia_cpp_script_assembly.dll
)