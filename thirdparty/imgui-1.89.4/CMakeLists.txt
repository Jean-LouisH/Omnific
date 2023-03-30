# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

file(GLOB_RECURSE imgui_sources "./[a-zA-Z]*.cpp")
file(GLOB_RECURSE imgui_headers "src/[a-zA-Z]*.h")
file(GLOB_RECURSE imgui_cpp_sources "./misc/cpp/[a-zA-Z]*.cpp")
file(GLOB_RECURSE imgui_cpp_headers "./misc/cpp/[a-zA-Z]*.h")
file(GLOB_RECURSE imgui_fonts_sources "./misc/fonts/[a-zA-Z]*.cpp")
file(GLOB_RECURSE imgui_freetype_sources "./misc/freetype/[a-zA-Z]*.cpp")
file(GLOB_RECURSE imgui_freetype_headers "./misc/freetype/[a-zA-Z]*.h")
file(GLOB_RECURSE imgui_backends_sources "./backends/[a-zA-Z]*.cpp")
file(GLOB_RECURSE imgui_backends_headers "./backends/[a-zA-Z]*.h")
file(GLOB_RECURSE imgui_single_file_headers "./misc/single_file/[a-zA-Z]*.h")


include_directories(
	 "."
     "../SDL2-2.26.2/include"
     "../freetype-2.11.1/include"
)

add_library(imgui STATIC
    ${imgui_sources}
    ${imgui_headers}
    ${imgui_cpp_sources}
    ${imgui_cpp_headers}
    ${imgui_fonts_sources}
    ${imgui_freetype_sources}
    ${imgui_freetype_headers}
    ${imgui_backends_sources}
    ${imgui_backends_headers}
    ${imgui_single_file_headers}
)
