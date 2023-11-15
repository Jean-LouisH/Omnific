
cmake_minimum_required (VERSION 3.8)

add_executable(application 
	  "application/main.cpp"
 )

 add_executable(dedicated_gpu_application 
	  "dedicated_gpu_application/main.cpp"
	  "gpu.hpp"
 )

add_dependencies(application
	omnia_engine
)

add_dependencies(dedicated_gpu_application
	omnia_engine
)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.26.2/lib/x64)

include_directories(
		../../thirdparty/SDL2-2.28.5/include
		../../thirdparty/SDL2_mixer-2.6.3/include
		../../thirdparty/SDL2_ttf-2.0.18/include
		../../thirdparty/yaml-cpp-0.6.3/include
		../../thirdparty/glad/include
		../../thirdparty/python37/include
		../../thirdparty/pybind11-2.11.1/include
		../../thirdparty/glm-0.9.9.8/glm
		../../thirdparty/tinygltf-2.8.18/include
		../../thirdparty/zlib-1.2.11/include
		../../thirdparty/stb_image_write-1.16/include
		../../thirdparty/stb_image-2.27/include
		../../thirdparty/libretti/include
		../engine
		.
)

target_link_libraries (application
		omnia_engine
		${SDL2}
)

target_link_libraries (dedicated_gpu_application
		omnia_engine
		${SDL2}
)