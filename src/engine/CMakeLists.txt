
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE lilliputian_engine_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE lilliputian_engine_headers "[a-zA-Z]*.hpp")

add_library(lilliputian_engine SHARED
	  ${lilliputian_engine_sources}
	  ${lilliputian_engine_headers}
)

add_dependencies (lilliputian_engine
	yaml_cpp
)

 add_definitions(/bigobj)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.0.12/lib/x64)
find_library(SDL2_Image SDL2_image.lib PATHS ../../thirdparty/SDL2_image-2.0.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../../thirdparty/SDL2_mixer-2.0.4/lib/x64)
find_library(SDL2_TTF SDL2_ttf.lib PATHS ../../thirdparty/SDL2_ttf-2.0.15/lib/x64)
find_library(Python37 python37.lib PATHS ../../thirdparty/python37/lib)

include_directories(
		../../thirdparty/SDL2-2.0.12/include
		../../thirdparty/SDL2_image-2.0.5/include
		../../thirdparty/SDL2_mixer-2.0.4/include
		../../thirdparty/SDL2_ttf-2.0.15/include
		../../thirdparty/yaml-cpp-0.6.3/include
		../../thirdparty/python37/include
		../../thirdparty/pybind11-2.6.2/include
		.
)

target_link_libraries (lilliputian_engine
		yaml_cpp
		${SDL2}
		${SDL2_Image}
		${SDL2_Mixer}
		${SDL2_TTF}
		${Python37}
)

add_definitions(-D_LILLIPUTIAN_ENGINE_EXPORTS)

add_custom_command(TARGET lilliputian_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lilliputian_engine> ${PROJECT_BINARY_DIR}/main/lilliputian_engine.dll
)