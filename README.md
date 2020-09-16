# [Lilliputian](https://github.com/Jean-LouisH/Lilliputian)

![screenshot](Documentation/Images/Screenshot.gif)

**This project is still a work in progress**

[Lilliputian](https://github.com/Jean-LouisH/Lilliputian) is **a tiny 2D game engine library** in which a complete engine link library is provided for the user to create small 2D games. This is achieved by defining scene trees with YAML files and writing C++ game scripts with a `Lilliputian::ScriptingAPIs` object. 

It is named after the tiny inhabitants of the fictional Lilliput island that appears in Jonathan Swift's 1726 novel "Guilliver's Travels." This reflects the objective of keeping the engine as small as necessary.

It is being developed to replace the archived and unstable [Suprannua Engine](https://github.com/Jean-LouisH/SuprannuaEngine/blob/master/README.md) framework without being as large and aimless as the experimental [Lania](https://github.com/Jean-LouisH/Lania/blob/master/README.md) application. To achieve this, it will reuse code from my [SDL Sub-Engines](https://github.com/Jean-LouisH/SDLSubEngines), [Libretti](https://github.com/Jean-LouisH/Libretti/blob/master/README.md) and [Lania](https://github.com/Jean-LouisH/Lania/blob/master/README.md) repositories. 

## Target Features

* An API for User Defined Scripting and Scene Management
* Mouse, Keyboard and Game Controller Support
* Haptic Feedback Support
* Default or User Defined Input Action Mapping (To Link Game Controller and Keyboard Inputs to the Same Logic)
* JPG and PNG Support for Sprites
* Spritesheets and Animation
* Multiple and Independent 2D Scene Layers (For Backgrounds, Game Environment, HUD, etc)
* Stacked Scenes (For Saved States on Scene Transitions)
* Procedural Geometric Primitive Objects (Rectangles, Triangles, Regular Polygons)
* Physics, Collision Detection and Response
* OGG and WAV Support for Audio Sources
* [Libretti](https://github.com/Jean-LouisH/Libretti/blob/master/README.md) Audio Waveform Generation and Scripting

## Compilation Instructions

* Requires the [SDL2](https://www.libsdl.org/), [SDL2_image](https://www.libsdl.org/projects/SDL_image/), and  [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) libraries for your development platform.
* Include this source directory in CMake to generate a project from the top level CMakeLists.txt. 
* Set the `CMAKE_CONFIGURATION_TYPES` to the build configuration you want. 
* Use your C++ IDE or compiler of choice to build the Lilliputian library file from the project. In the debug build, you also have the optional TestGame project to create a test executable for it.

## Game Development Instructions

* Create your empty C++ game project.
* Add your Lilliputian build as a link library and dependency for your project. 
* Add SDL2, SDL2_image and SDL2_mixer as target link libraries.
* Declare the `Lilliputian::Engine` object in a function (typically your `main()` entry point). In order to construct itself, the Engine object requires a game title, window width and height, and an entry scene name of the YAML file that you want your game to load first. If your game will have scripts, call the `addScriptCompilerFunction()` method from the Engine object. A script compiler is a callback of a definer function where you will declare all the script objects and their attached C++ script functions. If you wish to define all scripts in one definer function, then you can pass just one, but you also have the choice of passing as many as you wish. The callback definer functions should receive a reference to the engine's `Lilliputian::ScriptRegistry` object, so their signatures should resemble `void myCompilerFunction(Lilliputian::ScriptRegistry&)`.
* When you add C++ script functions to script objects, they should receive a reference to the engine's `Lilliputian::ScriptingAPIs` so the APIs and engine data can be accessed and editted, respectively. The signature should then resemble `void myScriptFunction(Lilliputian::ScriptingAPIs&)`.
* In the main function where you declared the `Lilliputian::Engine` object, call the method `run()` last and the engine will execute everything and handle the lower level details for you, including terminating safely.

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