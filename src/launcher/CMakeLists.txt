
cmake_minimum_required (VERSION 3.8)

project("omnific_launcher")

add_executable(omnific_launcher
	  "main.cpp"
 )

if(MINGW AND CMAKE_CROSSCOMPILING)
	set(MINGW_BIN "/usr/x86_64-w64-mingw32/sys-root/mingw/bin")

	set(RUNTIME_DEPS
		"${MINGW_BIN}/libgcc_s_seh-1.dll"
		"${MINGW_BIN}/libwinpthread-1.dll"
		"${MINGW_BIN}/libstdc++-6.dll"
	)

	add_custom_command(TARGET omnific_launcher POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${RUNTIME_DEPS}
		${PROJECT_BINARY_DIR}
		COMMENT "Deploying Windows/MinGW Runtime DLLs..."
	)
endif()
