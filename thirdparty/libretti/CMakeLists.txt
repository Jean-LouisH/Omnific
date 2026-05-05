
cmake_minimum_required (VERSION 3.8)

project ("libretti")

file(GLOB_RECURSE libretti_sources "[a-zA-Z]*.c")
file(GLOB_RECURSE libretti_headers "include/[a-zA-Z]*.h")

add_definitions(-DLIBRETTI_DATA_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/data/")

add_library(libretti STATIC
	  ${libretti_sources}
	  ${libretti_headers})


if (MSVC)
	add_definitions(/bigobj)
else ()
	if (MINGW)
		target_compile_options(libretti PRIVATE "-Wa,-mbig-obj")
	endif ()
endif ()

target_include_directories(libretti PRIVATE
		./include
		./thirdparty/dr_wav
		.
)

add_definitions(-D_LIBRETTI_EXPORTS_BUILD)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()