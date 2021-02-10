# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE lilliputian_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE lilliputian_headers "[a-zA-Z]*.hpp")

add_executable(lilliputian 
	  ${lilliputian_sources}
	  ${lilliputian_headers}
)

add_dependencies (lilliputian
	yaml_cpp
)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.0.12/lib/x64)
find_library(SDL2_Image SDL2_image.lib PATHS ../../thirdparty/SDL2_image-2.0.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../../thirdparty/SDL2_mixer-2.0.4/lib/x64)
find_library(SDL2_TTF SDL2_ttf.lib PATHS ../../thirdparty/SDL2_ttf-2.0.15/lib/x64)

include_directories(
		../../thirdparty/SDL2-2.0.12/include
		../../thirdparty/SDL2_image-2.0.5/include
		../../thirdparty/SDL2_mixer-2.0.4/include
		../../thirdparty/SDL2_ttf-2.0.15/include
		../../thirdparty/yaml-cpp-0.6.3/include
		.
)

target_link_libraries (lilliputian
		yaml_cpp
		${SDL2}
		${SDL2_Image}
		${SDL2_Mixer}
		${SDL2_TTF}
)