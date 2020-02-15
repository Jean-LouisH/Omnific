# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(LILLIPUTIAN_SOURCES
		Lilliputian.hpp
		Core/Core.hpp
		Core/Core.cpp
		Editor/Editor.hpp
		Editor/Editor.cpp
		Game/ActionInputs.cpp
		Game/ActionInputs.hpp
		Game/Entity2D.cpp
		Game/Entity2D.hpp
		Game/Game.hpp
		Game/Game.cpp
		Game/Scene2D.cpp
		Game/Scene2D.hpp
		Game/Script.cpp
		Game/Script.hpp
		ScriptingAPI/GameAPI.cpp
		ScriptingAPI/GameAPI.hpp
		ScriptingAPI/InputAPI.cpp
		ScriptingAPI/InputAPI.hpp
		ScriptingAPI/ScriptingAPI.hpp
		ScriptingAPI/ScriptingAPI.cpp
		Utilities/Definitions/Constants.hpp
		Utilities/Definitions/Aliases.hpp
		Utilities/Mathematics/AABB2D.hpp
		Utilities/Mathematics/Colour.cpp
		Utilities/Mathematics/Colour.hpp
		Utilities/Mathematics/Mathematics.hpp
		Utilities/Mathematics/Rectangle.hpp
		Utilities/Mathematics/Transform2D.hpp
		Utilities/Mathematics/Vector2.hpp
)

# Add source to this project's executable.
add_library(Lilliputian STATIC ${LILLIPUTIAN_SOURCES})

# TODO: Add tests and install targets if needed.
