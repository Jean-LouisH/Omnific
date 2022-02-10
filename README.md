# [Omnific Engine](https://github.com/Jean-LouisH/Omnific_Engine)

![screenshot](Screenshot.png)

**This project is still a work in progress**

[Omnific Engine](https://github.com/Jean-LouisH/Omnific_Engine) is a game engine I'm developing for fun, study and technical demonstrations. It is designed to be general purpose, data-driven, lightweight, and suitable also for interactive, multimedia applications. For now it is only targeted to PC Windows and Linux desktop platorms.

## Notable Features

- 2D/3D Entities
- Data-oriented Entity-Component-System (ECS) architecture
- C++ and Python scripting
- OpenGL Rendering Context
- Game Controller and Haptic Feedback support

## Notable Planned Features

- UI Framework built with UI Components and the UI System
- An Editor Application built with the Engine, C++ and Python scripting
- Multithreaded Systems
- Physically Based Rendering
- Vulkan Rendering Context
- Acoustic Audio Simulation

## Compilation Instructions

* Requires the [SDL2](https://www.libsdl.org/), [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) and Python 3.7 libraries for your development platform.
* Include this source directory in CMake to generate a project from the top level CMakeLists.txt. 
* Set the `CMAKE_CONFIGURATION_TYPES` to the build configuration you want. 
* Use your C++ IDE or compiler of choice to build Esi and Tests from the project.

## Documentation

* [Objectives](Documentation/Objectives/Objectives.md)
* [Design](Documentation/Design/Design.md)
* [End User Manual](Documentation/End_User_Manual/End_User_Manual.md)
* [Scripting API](Documentation/End_User_Manual/Scripting_API/Scripting_API.md)

## Demos

Simple Clock

![screenshot](SimpleClock.png)

## License

[MIT License](LICENSE)