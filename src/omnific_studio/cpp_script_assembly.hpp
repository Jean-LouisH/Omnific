// // MIT License
// // 
// // Copyright (c) 2020 Jean-Louis Haywood
// // 
// // Permission is hereby granted, free of charge, to any person obtaining a copy
// // of this software and associated documentation files (the "Software"), to deal
// // in the Software without restriction, including without limitation the rights
// // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// // copies of the Software, and to permit persons to whom the Software is
// // furnished to do so, subject to the following conditions:
// // 
// // The above copyright notice and this permission notice shall be included in all
// // copies or substantial portions of the Software.
// // 
// // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// // SOFTWARE.

// #pragma once

// #include <scene/scene_layer.hpp>
// #include <queue>
// #include <scene/components/script_collection.hpp>

// #if defined (_WIN32)
// #if defined(_OMNIFIC_CPP_SCRIPT_ASSEMBLY_EXPORTS)
// #define  OMNIFIC_CPP_SCRIPT_ASSEMBLY_API extern "C" __declspec(dllexport)
// #else
// #define  OMNIFIC_CPP_SCRIPT_ASSEMBLY_API extern "C" __declspec(dllimport)
// #endif
// #else
// #define OMNIFIC_CPP_SCRIPT_ASSEMBLY_API
// #endif

// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void loadScriptInstances();
// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void onStart();
// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void onInput();
// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void onEarly();
// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void onLogic();
// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void onCompute();
// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void onLate();
// OMNIFIC_CPP_SCRIPT_ASSEMBLY_API void onFinish();

// void bindAndCall(std::shared_ptr<Omnific::ScriptCollection> scriptCollection,
// 	Omnific::SceneLayerID sceneLayerID,
// 	Omnific::EntityID entityID,
// 	std::string methodName);

// void executeQueuedMethods(
// 	std::queue<Omnific::EntityID> entityQueue,
// 	std::shared_ptr<Omnific::SceneLayer> sceneLayer,
// 	std::string methodName);

// void executeUpdateMethods(
// 	std::shared_ptr<Omnific::SceneLayer> sceneLayer,
// 	std::string methodName);