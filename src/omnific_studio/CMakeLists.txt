
cmake_minimum_required (VERSION 3.8)

project ("omnific_studio")

file(GLOB_RECURSE omnific_studio_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnific_studio_headers "[a-zA-Z]*.hpp")

add_executable(omnific_studio
	  ${omnific_studio_sources}
	  ${omnific_studio_headers})

add_dependencies(omnific_studio
	omnific_engine
)

target_include_directories(omnific_studio PRIVATE
		../omnific_engine
		.
)

target_link_libraries (omnific_studio
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

target_link_options(omnific_studio PUBLIC ${SUBSYSTEM_LINKER_OPTIONS})