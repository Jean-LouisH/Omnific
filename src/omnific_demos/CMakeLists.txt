
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE omnific_demos_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnific_demos_headers "[a-zA-Z]*.hpp")

project ("omnific_demos")

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

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (WIN32)
        # /ENTRY:mainCRTStartup keeps the same "main" function instead of requiring "WinMain"
        set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS"  "/ENTRY:mainCRTStartup")
   else()
       set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")
    endif()
endif()

target_link_options(omnific_demos PUBLIC ${SUBSYSTEM_LINKER_OPTIONS})