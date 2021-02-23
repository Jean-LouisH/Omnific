
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE lilliputian_main_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE lilliputian_main_headers "[a-zA-Z]*.hpp")

add_executable(lilliputian_main 
	  ${lilliputian_main_sources}
	  ${lilliputian_main_headers}
 )

add_dependencies(lilliputian_main
	lilliputian_engine
)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.0.12/lib/x64)

include_directories(
		../../thirdparty/SDL2-2.0.12/include
		../../thirdparty/SDL2_image-2.0.5/include
		../../thirdparty/SDL2_mixer-2.0.4/include
		../../thirdparty/SDL2_ttf-2.0.15/include
		../../thirdparty/yaml-cpp-0.6.3/include
		../lilliputian_engine
		.
)

target_link_libraries (lilliputian_main
		lilliputian_engine
		${SDL2}
)