# [Omnia](https://github.com/Jean-LouisH/Omnia)

![Intro Screenshot](debug.png)

**This project is in pre-alpha**

[Omnia](https://github.com/Jean-LouisH/Omnia) is a game engine I'm developing for fun, study and technical demonstrations. It doubles as a real-time software framework for interactive applications. It is designed to be general purpose, data-driven, and lightweight.

The name "Omnia" stems from the prefix "Omni-" which means "all." It is loosely based on the term "Omnific" which means ["all-creating"](https://www.merriam-webster.com/dictionary/omnific).

# Table of Contents

* [Notable Features](#notable-features)
* [Compilation Instructions](#compilation-instructions)
* [Manual](#manual)
* [Demos](#demos)
* [License](#license)

# Notable Features

- Project-based, data-driven Applications

![omnia_project](omnia_project.png)

- Scenes of multiple SceneTree layers (2D and 3D)

![scene_tree_layers](scene_tree_layers.gif)

- SceneTree EventBus for a publish/subscribe architecture

![event_bus](event_bus.gif)

- C++ scripting via Native shared library builds
- Python scripting via a Virtual Machine
- Entity-Component-System (ECS) architecture
- OpenGL Rendering Context with GLSL
- Game Controller and Haptic Feedback support
- Multithreaded Systems

# Compilation Instructions

* Requires the [SDL2](https://www.libsdl.org/), [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) and [Embeddable Python 3.7](https://www.python.org/downloads/release/python-370/) libraries for your development platform.
* To be able to install third-party packages in Embeddable Python 3.7, uncomment the last line in python37._pth. Then download [get-pip.py](https://bootstrap.pypa.io/get-pip.py), place it in the embeddable python folder, and run `> python get-pip.py`. Install packages to the embeddable folder with `> python -m pip install package-name`.
* Include this source directory in CMake to generate a project from the top level CMakeLists.txt. 
* Set the `CMAKE_CONFIGURATION_TYPES` to the build configuration you want. 
* Use your C++ IDE or compiler of choice to build Omnia and Tests from the project.

# Manual



## Demos

Loading Models

![screenshot](loading_models.png)

## License

[MIT License](LICENSE)