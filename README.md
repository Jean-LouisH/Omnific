# [Lilliputian](https://github.com/Jean-LouisH/Lilliputian)

![screenshot](Documentation/Images/Screenshot.gif)

[Lilliputian](https://github.com/Jean-LouisH/Lilliputian) is **a tiny 2D game engine library** in which a complete engine link library is provided for the user to create small 2D games. This is achieved by updating engine scene graphs and writing C++ game scripts. 

It is named after the tiny inhabitants of the fictional Lilliput island that appears in Jonathan Swift's 1726 novel "Guilliver's Travels." This reflects the objective of keeping the engine as small as necessary.

It is being developed to replace the archived and unstable [Suprannua Engine] (https://github.com/Jean-LouisH/SuprannuaEngine/blob/master/README.md) framework without being as large and aimless as the experimental [Lania](https://github.com/Jean-LouisH/Lania/blob/master/README.md) application. To achieve this, it will reuse code from my [SDL Sub-Engines](https://github.com/Jean-LouisH/SDLSubEngines), [Libretti](https://github.com/Jean-LouisH/Libretti/blob/master/README.md) and [Lania](https://github.com/Jean-LouisH/Lania/blob/master/README.md) repositories. 

This also started as a practical scenario to learn how to use CMake through CMakeLists and to use unit testing frameworks like Google Test.

## Target Features

* Entity Component System Architecture
* Multiple, Independent and Layered 2D Subscenes(For Backgrounds, Game Environment, HUD, etc)
* Geometric Primitive Objects (Rectangles, Triangles, Regular Polygons)
* An API for User Entity and Scene Management
* Mouse, Keyboard and Game Controller Support
* Input Action Mapping (Default Provided)
* JPG and PNG for Sprites
* Spritesheets and Animation
* OGG for audio sources
* [Libretti](https://github.com/Jean-LouisH/Libretti/blob/master/README.md) audio waveform generation and scripting
* Physics, Collision Detection and Response

## Compilation Instructions

* Include this source directory in CMake to generate a project from the top level CMakeLists.txt. 
* Set the CMAKE_CONFIGURATION_TYPES to your target build configuration. 
* Use your C++ IDE or compiler of choice to build the Lilliputian library file from the project. In the debug build, you also have the TestGame project to create a test executable for it.

## Game Development Instructions

* Create your own empty C++ project.
* Add your Lilliputian build as a link library and dependency for your project. 
* Declare a Lilliputian::API object in a function (typically main entry point) and via the object's method, attach a scene initializer subroutine (a function or method of your choice). In that subroutine you should at least define the main scene and attach all other executable scenes by manipulating Entity Component objects. Attach C++ scripts to those objects. These instructions and other API guidelines can be found in the API documentation.

## Documentation

## Demos

## License

[MIT License](LICENSE)