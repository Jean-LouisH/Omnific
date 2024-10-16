
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE omnific_demos_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnific_demos_headers "[a-zA-Z]*.hpp")

add_executable(omnific_demos 
	  ${omnific_demos_sources}
      ${omnific_demos_headers}
 )

add_dependencies(omnific_demos
	omnific_engine
)

target_include_directories(omnific_demos PRIVATE
		../omnific_engine
		.
)

target_link_libraries (omnific_demos
		omnific_engine
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-D_DEBUG)
endif()

string(REGEX MATCH "[0-9]+\\.[0-9]+" Python_VERSION_MAJOR_MINOR "${PYTHON_VERSION_CACHED}")
set (PYTHON_DIR_NAME "python${Python_VERSION_MAJOR_MINOR}")

if (WIN32)
	set (VCPKG_PYTHON_SOURCE_PATH ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/tools/python3/Lib)
	set (PYTHON_LIB_DESTINATION_PATH ${PROJECT_BINARY_DIR}/src/omnific_demos/Lib)
else ()
	set (VCPKG_PYTHON_SOURCE_PATH ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/lib/${PYTHON_DIR_NAME}/)
	set (PYTHON_LIB_DESTINATION_PATH ${PROJECT_BINARY_DIR}/src/omnific_demos/${PYTHON_DIR_NAME}/)
endif()

	# Create the python directory in the out folder and copy the necessary files
	add_custom_command(TARGET omnific_demos POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_LIB_DESTINATION_PATH}
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${VCPKG_PYTHON_SOURCE_PATH} ${PYTHON_LIB_DESTINATION_PATH}
	)