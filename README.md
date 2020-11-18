# [Lilliputian](https://github.com/Jean-LouisH/Lilliputian)

![screenshot](Documentation/Images/Screenshot.gif)

**This project is still a work in progress**

[Lilliputian](https://github.com/Jean-LouisH/Lilliputian) is **a tiny 2D game engine and IDE** for introductory game development, prototyping and programming learning. Its sole purpose is to provide the user with the entire development process in a single application and make it as simple as possible.

It is named after the tiny inhabitants of the fictional Lilliput island that appears in Jonathan Swift's 1726 novel "Guilliver's Travels." This reflects the objective of keeping the engine as small as necessary.

## Target Features

* Scene Editor
* IDE and VM for "LilliScript", a Custom Typed Language Heavily Based on Pseudocode, with Debugging and Testing Support.
* In-Editor Prototyping
* Game Executable Exporting
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
* Use your C++ IDE or compiler of choice to build Lilliputian and LilliputianTests from the project.

## Documentation

* [Manual](Documentation/Manual.md)
* [API](Documentation/API.md)
* [Architecture](Documentation/Architecture.md)

## Demos

## Contributing

* Jean-Louis Haywood (Jean-LouisH)

## License

[MIT License](LICENSE)