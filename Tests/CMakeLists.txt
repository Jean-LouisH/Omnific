# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE tests_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE tests_headers "[a-zA-Z]*.hpp")

add_executable(LilliputianTests 
	  ${tests_sources}
	  ${tests_headers}
)

include_directories(
		../Lilliputian
		../ThirdParty/SDL2-2.0.12/include
		../ThirdParty/SDL2_image-2.0.5/include
		../ThirdParty/SDL2_mixer-2.0.4/include
		../ThirdParty/SDL2_ttf-2.0.15/include
		../ThirdParty/yaml-cpp-0.6.3/include
		../ThirdParty/Libretti/include
)

add_dependencies (Tests 
		Lilliputian
)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../ThirdParty/SDL2-2.0.12/lib/x64)

target_link_libraries (Tests
		${SDL2}
)
