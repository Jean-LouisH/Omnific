
cmake_minimum_required (VERSION 3.8)

add_executable(application 
	  "main.cpp"
 )

add_dependencies(application
	omnia_engine
)

target_include_directories(application PRIVATE
		../engine
		.
)

target_link_libraries (application
		omnia_engine
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (WIN32)
        # /ENTRY:mainCRTStartup keeps the same "main" function instead of requiring "WinMain"
        set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS"  "/ENTRY:mainCRTStartup")
   else()
       set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")
    endif()
endif()

target_link_options(application PUBLIC ${SUBSYSTEM_LINKER_OPTIONS})