
cmake_minimum_required (VERSION 3.8)

project ("omnific_studio")

set (TARGET_NAME "omnific_studio")

file(GLOB_RECURSE ${TARGET_NAME}_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE ${TARGET_NAME}_headers "[a-zA-Z]*.hpp")

add_executable(${TARGET_NAME}
	${${TARGET_NAME}_sources}
	${${TARGET_NAME}_headers}
 )

add_dependencies(${TARGET_NAME}
	omnific_engine
)

target_include_directories(${TARGET_NAME} PRIVATE
		../omnific_engine
		.
)

target_link_libraries (${TARGET_NAME}
		omnific_engine
)

add_definitions(-DPROJECT_SOURCE_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/")
add_definitions(-DPROJECT_APP_DATA_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/app_data/")
add_definitions(-DPROJECT_BUILD_DIRECTORY="${CMAKE_CURRENT_BINARY_DIR}/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(USE_FLAGS "-sWASM=1 -sUSE_SDL=2 -sUSE_SDL_TTF=2 --embed-file ${CMAKE_CURRENT_SOURCE_DIR}/app_data")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set (THIRD_PARTY_BINS) 
else ()
	message (STATUS "Demo Python version: ${PYTHON_VERSION_CACHED}")
	string(REGEX MATCH "[0-9]+\\.[0-9]+" Python_VERSION_MAJOR_MINOR "${PYTHON_VERSION_CACHED}")
	set (PYTHON_DIR_NAME "python${Python_VERSION_MAJOR_MINOR}")

	if (WIN32)
		set (VCPKG_PYTHON_LIB_SOURCE_PATH ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/tools/python3/Lib)
		set (PYTHON_LIB_DESTINATION_DIR ${PROJECT_BINARY_DIR}/Lib)
		set (PYTHON_LIB_DESTINATION_PATH ${PYTHON_LIB_DESTINATION_DIR})
	else ()
		set (VCPKG_PYTHON_LIB_SOURCE_PATH ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/lib/${PYTHON_DIR_NAME}/)
		set (PYTHON_LIB_DESTINATION_DIR ${PROJECT_BINARY_DIR}/lib)
		set (PYTHON_LIB_DESTINATION_PATH ${PYTHON_LIB_DESTINATION_DIR}/${PYTHON_DIR_NAME}/)
	endif()

	# Create the python directory in the out folder and copy the necessary files
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_LIB_DESTINATION_DIR}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_LIB_DESTINATION_PATH}
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${VCPKG_PYTHON_LIB_SOURCE_PATH} ${PYTHON_LIB_DESTINATION_PATH}
	)
endif()