cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE omnific_engine_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnific_engine_headers "[a-zA-Z]*.hpp")

if (WIN32)
   set(STATIC_LIBRARY_EXTENSION "lib")
   set(SHARED_LIBRARY_EXTENSION "dll")
   set(LIBRARY_PREFIX "")
   add_definitions(/bigobj)
else()
	set(STATIC_LIBRARY_EXTENSION "a")
	set(SHARED_LIBRARY_EXTENSION "so")
	set(LIBRARY_PREFIX "lib")
endif()

set (LIBRARY_TYPE "STATIC")

add_library(omnific_engine ${LIBRARY_TYPE}
	  ${omnific_engine_sources}
	  ${omnific_engine_headers})

set(THIRD_PARTY_DIR "../../thirdparty")

set(THIRD_PARTY_PROJECTS
	glad
	libretti
	ufbx
	yaml_cpp
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-sUSE_SDL=2 -sUSE_SDL_TTF=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
	set (THIRD_PARTY_BINS) 
else()
	find_package(OpenAL REQUIRED)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_ttf REQUIRED)
	find_package(Python COMPONENTS Interpreter Development REQUIRED)
endif()

add_dependencies (omnific_engine 
	${THIRD_PARTY_PROJECTS}
)

target_include_directories(omnific_engine PUBLIC
	${THIRD_PARTY_DIR}/glad/include
	${THIRD_PARTY_DIR}/glm
	${THIRD_PARTY_DIR}/libretti/include
	${THIRD_PARTY_DIR}/pybind11/include
	${THIRD_PARTY_DIR}/stb_image
	${THIRD_PARTY_DIR}/tinygltf
	${THIRD_PARTY_DIR}/tinyobjloader
	${THIRD_PARTY_DIR}/ufbx
	${THIRD_PARTY_DIR}/yaml-cpp/include
	.
)

target_link_libraries (omnific_engine 
	${THIRD_PARTY_PROJECTS}
	OpenAL::OpenAL
	SDL2::SDL2
	SDL2_ttf::SDL2_ttf
	Python::Python
)

if (${LIBRARY_TYPE} STREQUAL "SHARED")
	add_definitions(-D_OMNIFIC_ENGINE_EXPORTS)
endif ()
add_definitions(-D_OMNIFIC_ENGINE_EXPORTS_BUILD)
add_definitions(-D_OMNIFIC_APPLICATION_EXPORTS)
add_definitions(-D_OMNIFIC_APPLICATION_EXPORTS_BUILD)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()

add_custom_command(TARGET omnific_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:omnific_engine> ${PROJECT_BINARY_DIR}/src/omnific_demos/omnific_engine.${SHARED_LIBRARY_EXTENSION} 
)

add_custom_command(TARGET omnific_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:omnific_engine> ${PROJECT_BINARY_DIR}/src/omnific_studio/omnific_engine.${SHARED_LIBRARY_EXTENSION} 
)