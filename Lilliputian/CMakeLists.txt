# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE lilliputian_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE lilliputian_headers "[a-zA-Z]*.hpp")

add_executable(Lilliputian 
	  ${lilliputian_sources}
	  ${lilliputian_headers}
)

add_dependencies (Lilliputian
	YAML_CPP
	Libretti
)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../ThirdParty/SDL2-2.0.12/lib/x64)
find_library(SDL2_Image SDL2_image.lib PATHS ../ThirdParty/SDL2_image-2.0.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../ThirdParty/SDL2_mixer-2.0.4/lib/x64)
find_library(SDL2_TTF SDL2_ttf.lib PATHS ../ThirdParty/SDL2_ttf-2.0.15/lib/x64)

include_directories(
		../ThirdParty/SDL2-2.0.12/include
		../ThirdParty/SDL2_image-2.0.5/include
		../ThirdParty/SDL2_mixer-2.0.4/include
		../ThirdParty/SDL2_ttf-2.0.15/include
		../ThirdParty/yaml-cpp-0.6.3/include
		../ThirdParty/Libretti/include
)

target_link_libraries (Lilliputian
		YAML_CPP
		Libretti
		${SDL2}
		${SDL2_Image}
		${SDL2_Mixer}
		${SDL2_TTF}
)