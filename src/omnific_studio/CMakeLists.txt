
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
		../engine
		.
)

target_link_libraries (omnific_studio
		omnific_engine
)