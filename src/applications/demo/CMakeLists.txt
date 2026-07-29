
cmake_minimum_required (VERSION 3.8)

project ("omnific_demo")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (TARGET_NAME "omnific_demo")

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
    set(USE_FLAGS "-sWASM=1 -sALLOW_MEMORY_GROWTH=1 -sOFFSCREEN_FRAMEBUFFER=1 -sUSE_WEBGL2=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/app_data")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")

	set_target_properties(${TARGET_NAME} PROPERTIES
		OUTPUT_NAME "index"
		SUFFIX ".html"
	)

	set(GITHUB_PAGES_DIR "${CMAKE_SOURCE_DIR}/docs")

	add_custom_command(TARGET omnific_demo POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${GITHUB_PAGES_DIR}"

		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"$<TARGET_FILE_DIR:omnific_demo>/index.html"
			"${GITHUB_PAGES_DIR}/index.html"

		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"$<TARGET_FILE_DIR:omnific_demo>/index.js"
			"${GITHUB_PAGES_DIR}/index.js"

		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"$<TARGET_FILE_DIR:omnific_demo>/index.wasm"
			"${GITHUB_PAGES_DIR}/index.wasm"

		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"$<TARGET_FILE_DIR:omnific_demo>/index.data"
			"${GITHUB_PAGES_DIR}/index.data"

		VERBATIM
	)
else ()
	message (STATUS "Demo Python version: ${PYTHON_VERSION_CACHED}")
	string(REGEX MATCH "[0-9]+\\.[0-9]+" Python_VERSION_MAJOR_MINOR "${PYTHON_VERSION_CACHED}")
	set (PYTHON_DIR_NAME "python${Python_VERSION_MAJOR_MINOR}")

	if(MINGW AND CMAKE_CROSSCOMPILING)
		set(MINGW_BIN "/usr/x86_64-w64-mingw32/sys-root/mingw/bin")

		set(PYTHON_RUNTIME_DEPS
			"${MINGW_BIN}/libpython3.11.dll"
			"${MINGW_BIN}/libgcc_s_seh-1.dll"
			"${MINGW_BIN}/libwinpthread-1.dll"
			"${MINGW_BIN}/libstdc++-6.dll"
		)

		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${PYTHON_RUNTIME_DEPS}
			${PROJECT_BINARY_DIR}
			COMMENT "Deploying Windows/MinGW Runtime DLLs..."
		)

		set(MINGW_PYTHON_LIB_DIR "/usr/x86_64-w64-mingw32/sys-root/mingw/lib/python3.11")
		set(PYTHON_LIB_DESTINATION_PATH "${PROJECT_BINARY_DIR}/lib/python3.11")

		# Copy the entire directory to your build folder
		add_custom_command(TARGET omnific_demo POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_LIB_DESTINATION_PATH}
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${MINGW_PYTHON_LIB_DIR}"
			"${PYTHON_LIB_DESTINATION_PATH}"
			COMMENT "Deploying Python Standard Library (os.py, etc)..."
		)
	else ()
		if (MSVC)
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
endif()