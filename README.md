# [Lilliputian](https://github.com/Jean-LouisH/Lilliputian)

![screenshot](Documentation/Images/Screenshot.gif)

**This project is still a work in progress**

[Lilliputian](https://github.com/Jean-LouisH/Lilliputian) is **a tiny 2D game engine library** in which a complete engine link library is provided for the user to create small 2D games. This is achieved by updating game scene trees with a `Lilliputian::EditorAPI` object and writing C++ game scripts with a `Lilliputian::ScriptingAPI` object. 

It is named after the tiny inhabitants of the fictional Lilliput island that appears in Jonathan Swift's 1726 novel "Guilliver's Travels." This reflects the objective of keeping the engine as small as necessary.

It is being developed to replace the archived and unstable [Suprannua Engine](https://github.com/Jean-LouisH/SuprannuaEngine/blob/master/README.md) framework without being as large and aimless as the experimental [Lania](https://github.com/Jean-LouisH/Lania/blob/master/README.md) application. To achieve this, it will reuse code from my [SDL Sub-Engines](https://github.com/Jean-LouisH/SDLSubEngines), [Libretti](https://github.com/Jean-LouisH/Libretti/blob/master/README.md) and [Lania](https://github.com/Jean-LouisH/Lania/blob/master/README.md) repositories. 

## Target Features

* An API for User Defined Scene Management and Scripting
* Mouse, Keyboard and Game Controller Support
* Input Action Mapping (Default Provided)
* JPG and PNG Support for Sprites
* Spritesheets and Animation
* OGG Support for Audio Sources
* Physics, Collision Detection and Response
* Multiple, Independent and Layered 2D Subscenes (For Backgrounds, Game Environment, HUD, etc)
* Geometric Primitive Objects (Rectangles, Triangles, Regular Polygons)
* [Libretti](https://github.com/Jean-LouisH/Libretti/blob/master/README.md) Audio Waveform Generation and Scripting

## Compilation Instructions

* Requires the appropriate [SDL2](https://www.libsdl.org/), [SDL2_image](https://www.libsdl.org/projects/SDL_image/), and  [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) libraries for your development platform.
* Include this source directory in CMake to generate a project from the top level CMakeLists.txt. 
* Set the `CMAKE_CONFIGURATION_TYPES` to the build configuration you want. 
* Use your C++ IDE or compiler of choice to build the Lilliputian library file from the project. In the debug build, you also have the TestGame project to create a test executable for it.

## Game Development Instructions

* Create your empty C++ game project.
* Add your Lilliputian build as a link library and dependency for your project. 
* Add SDL2, SDL2_image and SDL2_mixer as target link libraries.
* Declare the `Lilliputian::Core` object in a function (typically the `main()` entry point) and call the method `addSceneDefiner()` while passing a callback function or method of your choice. If you wish to define all scenes in one function, then you can pass just one, but you also have the choice of passing as many as you wish. The callback functions should receive a pointer to the core's `Lilliputian::EditorAPI` object, so their signatures should be `void myFunction(Lilliputian::EditorAPI*)`.
* In those callback functions, use the EditorAPI object to define `Lilliputian::Entity2D` game objects. Then add your game script callbacks to `Lilliputian::Script` objects, your script objects to your game objects and your game objects to your `Lilliputian::Scene` entry scene and all other executable scenes.
* When you add game script callbacks to script objects, they should receive a pointer to the core's `Lilliputian::ScriptingAPI` so the API and engine data can be accessed and editted, respectively.
* In the main function where you declared the `Lilliputian::Core` object, call the method `run()` and the engine will execute everything and handle the lower level details for you, including terminating safely.

These instructions and other API guidelines can be found in the API documentation.

## Documentation

* [Manual](Documentation/Manual.md)
* [API](Documentation/API.md)
* [Architecture](Documentation/Architecture.md)

## Demos

## Contributing

* Jean-Louis Haywood (Jean-LouisH)

## License

[MIT License](LICENSE)