
cmake_minimum_required(VERSION 3.14)

project ("omnia_tests")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/be03d00f5f0cc3a997d1a368bee8a1fe93651f48.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE omnia_tests_sources "[a-zA-Z]*.cpp")
file(GLOB_RECURSE omnia_tests_headers "[a-zA-Z]*.hpp")

add_executable(omnia_tests
	  ${omnia_tests_sources}
	  ${omnia_tests_headers})

add_dependencies(omnia_tests
	omnia_engine
)

 add_definitions(/bigobj)


include_directories(
		../src/engine
		.
)

target_link_libraries (omnia_tests
		omnia_engine
		GTest::gtest_main

)

include(GoogleTest)
gtest_discover_tests(omnia_tests)