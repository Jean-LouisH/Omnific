
cmake_minimum_required (VERSION 3.8)

add_executable(application 
	  "application/main.cpp"
 )

 add_executable(dedicated_gpu_application 
	  "dedicated_gpu_application/main.cpp"
	  "gpu.hpp"
 )

add_dependencies(application
	omnia_engine
)

add_dependencies(dedicated_gpu_application
	omnia_engine
)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.26.2/lib/x64)

target_include_directories(application PRIVATE
		../engine
		.
)

target_include_directories(dedicated_gpu_application PRIVATE
		../engine
		.
)

target_link_libraries (application
		omnia_engine
		${SDL2}
)

target_link_libraries (dedicated_gpu_application
		omnia_engine
		${SDL2}
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
target_link_options(dedicated_gpu_application PUBLIC ${SUBSYSTEM_LINKER_OPTIONS})