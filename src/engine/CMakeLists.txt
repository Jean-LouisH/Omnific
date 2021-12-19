
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE esi_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE esi_headers "[a-zA-Z]*.hpp")

add_library(esi SHARED
	  ${esi_sources}
	  ${esi_headers}
)

add_dependencies (esi
	yaml_cpp
	glad
)

 add_definitions(/bigobj)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.0.12/lib/x64)
find_library(SDL2_Image SDL2_image.lib PATHS ../../thirdparty/SDL2_image-2.0.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../../thirdparty/SDL2_mixer-2.0.4/lib/x64)
find_library(SDL2_TTF SDL2_ttf.lib PATHS ../../thirdparty/SDL2_ttf-2.0.15/lib/x64)
find_library(Python37 python37.lib PATHS ../../thirdparty/python37/lib)
find_library(ZLIB zlib.lib PATHS ../../thirdparty/zlib-1.2.11/lib)


include_directories(
		../../thirdparty/SDL2-2.0.12/include
		../../thirdparty/SDL2_image-2.0.5/include
		../../thirdparty/SDL2_mixer-2.0.4/include
		../../thirdparty/SDL2_ttf-2.0.15/include
		../../thirdparty/yaml-cpp-0.6.3/include
		../../thirdparty/python37/include
		../../thirdparty/pybind11-2.8.1/include
		../../thirdparty/glad/include
		../../thirdparty/glm-0.9.9.8/glm
		../../thirdparty/tinygltf-2.5.0/include
		../../thirdparty/zlib-1.2.11/include
		.
)

target_link_libraries (esi
		yaml_cpp
		glad
		${SDL2}
		${SDL2_Image}
		${SDL2_Mixer}
		${SDL2_TTF}
		${Python37}
		${ZLIB}

)

add_definitions(-D_ESI_EXPORTS)

add_custom_command(TARGET esi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:esi> ${PROJECT_BINARY_DIR}/main/esi.dll
)