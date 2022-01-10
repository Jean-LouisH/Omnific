# Esi native C++ scripting build

cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE esi_native_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE esi_native_headers "[a-zA-Z]*.hpp")


add_library(esi_native SHARED
	  ${esi_native_sources}
	  ${esi_native_headers}
)

 add_definitions(/bigobj)

include_directories(
		.
		../engine
)

target_link_libraries (esi_native

)

add_definitions(-D_ESI_NATIVE_EXPORTS)

add_custom_command(TARGET esi_native POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:esi_native> ${PROJECT_BINARY_DIR}/main/esi_native.dll
)