cmake_minimum_required (VERSION 3.8)

project ("omnific_engine")

file(GLOB_RECURSE omnific_engine_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnific_engine_headers "[a-zA-Z]*.hpp")

if (WIN32)
   add_definitions(/bigobj)
endif()

if (__EMSCRIPTEN__)
	add_definitions(-D_WEB_PLATFORM)
endif()

add_library(omnific_engine STATIC
	  ${omnific_engine_sources}
	  ${omnific_engine_headers})

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

set(THIRD_PARTY_PROJECTS
	glad
	ufbx
	yaml_cpp
)

message (STATUS "Engine detected system name: ${CMAKE_SYSTEM_NAME}")

if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(USE_FLAGS "-sWASM=1 -sUSE_SDL=2 -sUSE_SDL_TTF=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set (THIRD_PARTY_BINS) 

	target_link_libraries (omnific_engine 
		${THIRD_PARTY_PROJECTS}
	)
else()
	find_package(OpenAL REQUIRED)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_ttf REQUIRED)

	if (WIN32)
		set (SDL2_TTF_MODULE "SDL2_ttf::SDL2_ttf")
	else ()
		set (SDL2_TTF_MODULE "SDL2_ttf::SDL2_ttf-static")
	endif ()

	add_definitions(-DENABLE_PYTHON_BUILD)
	find_package(Python COMPONENTS Interpreter Development REQUIRED)
	find_package(pybind11 CONFIG REQUIRED)

	target_link_libraries (omnific_engine 
		${THIRD_PARTY_PROJECTS}
		OpenAL::OpenAL
		SDL2::SDL2
		${SDL2_TTF_MODULE}
		Python::Python
		pybind11::headers
	)
endif()

add_dependencies (omnific_engine 
	${THIRD_PARTY_PROJECTS}
)

target_include_directories(omnific_engine PUBLIC
	${THIRD_PARTY_DIR}/glad/include
	${THIRD_PARTY_DIR}/glm
	${THIRD_PARTY_DIR}/stb_image
	${THIRD_PARTY_DIR}/tinygltf
	${THIRD_PARTY_DIR}/tinyobjloader
	${THIRD_PARTY_DIR}/ufbx
	${THIRD_PARTY_DIR}/yaml-cpp/include
	.
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()

set (PYTHON_VERSION_CACHED "${Python_VERSION}" CACHE INTERNAL "")

add_definitions(-D_OMNIFIC_ENGINE_EXPORTS_BUILD)
add_definitions(-D_OMNIFIC_APPLICATION_EXPORTS)
add_definitions(-D_OMNIFIC_APPLICATION_EXPORTS_BUILD)