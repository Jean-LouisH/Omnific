cmake_minimum_required (VERSION 3.8)

project ("omnific_engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE omnific_engine_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnific_engine_headers "[a-zA-Z]*.hpp")

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

set(THIRD_PARTY_PROJECTS
	ufbx
	libretti
)

add_library(omnific_engine STATIC
	  ${omnific_engine_sources}
	  ${omnific_engine_headers})

message (STATUS "Engine detected system name: ${CMAKE_SYSTEM_NAME}")

find_package(glm CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	add_definitions(-D_WEB_PLATFORM)

	target_link_libraries (omnific_engine 
		${THIRD_PARTY_PROJECTS}
		glm::glm
		yaml-cpp::yaml-cpp
		SDL3::SDL3
		SDL3_ttf::SDL3_ttf
	)
else()
	set (THIRD_PARTY_PROJECTS ${THIRD_PARTY_PROJECTS} glad)

	if (MSVC)
		add_definitions(/bigobj)
		set (SDL3_TTF_MODULE "SDL3_ttf::SDL3_ttf")
	else ()
		if (MINGW)
			target_compile_options(omnific_engine PRIVATE "-Wa,-mbig-obj")
		endif ()
		set (SDL3_TTF_MODULE "SDL3_ttf::SDL3_ttf-static")
	endif ()

	add_definitions(-DENABLE_PYTHON_BUILD)

	if(MINGW AND CMAKE_CROSSCOMPILING)
		message(STATUS "Cross-compiling for MinGW: Manual Python Setup")

		set(MINGW_PYTHON_INCLUDE "/usr/x86_64-w64-mingw32/sys-root/mingw/include/python3.11")
		set(MINGW_PYTHON_LIB "/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libpython3.11.dll.a")

		if(NOT TARGET Python3::Module)
			add_library(Python3::Module INTERFACE IMPORTED)
			set_target_properties(Python3::Module PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES "${MINGW_PYTHON_INCLUDE}"
				INTERFACE_LINK_LIBRARIES "${MINGW_PYTHON_LIB}"
			)
		endif()

		if(NOT TARGET pybind11::module)
			add_library(pybind11::module INTERFACE IMPORTED)
			set_target_properties(pybind11::module PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES "${THIRD_PARTY_DIR}/pybind11"
				INTERFACE_LINK_LIBRARIES Python3::Module
			)
		endif()

		target_link_libraries (omnific_engine 
			${THIRD_PARTY_PROJECTS}
			SDL3::SDL3
			${SDL3_TTF_MODULE}
			Python3::Module
			pybind11::module
			yaml-cpp::yaml-cpp
			glm::glm
		)
	else()
		find_package(Python COMPONENTS Interpreter Development REQUIRED)
		find_package(pybind11 CONFIG REQUIRED)

		target_link_libraries (omnific_engine 
			${THIRD_PARTY_PROJECTS}
			SDL3::SDL3
			${SDL3_TTF_MODULE}
			Python::Python
			pybind11::headers
			yaml-cpp::yaml-cpp
			glm::glm
		)
	endif()
endif()

add_dependencies (omnific_engine 
	${THIRD_PARTY_PROJECTS}
)

target_include_directories(omnific_engine PUBLIC
	${THIRD_PARTY_DIR}/pybind11
	${THIRD_PARTY_DIR}/dr_wav
	${THIRD_PARTY_DIR}/glad/include
	${THIRD_PARTY_DIR}/stb_image
	${THIRD_PARTY_DIR}/stb_vorbis
	${THIRD_PARTY_DIR}/tinyobjloader
	${THIRD_PARTY_DIR}/ufbx
	${THIRD_PARTY_DIR}/libretti/include
	${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
	.
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()

set (PYTHON_VERSION_CACHED "${Python_VERSION}" CACHE INTERNAL "")

add_definitions(-D_OMNIFIC_ENGINE_EXPORTS_BUILD)
add_definitions(-D_OMNIFIC_APPLICATION_EXPORTS)
add_definitions(-D_OMNIFIC_APPLICATION_EXPORTS_BUILD)