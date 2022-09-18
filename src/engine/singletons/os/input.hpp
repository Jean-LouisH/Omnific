// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <utilities/aliases.hpp>
#include <glm/glm.hpp>
#include <set>
#include <engine_api.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API Input
	{
		friend class Engine;
	public:
		Input();

		bool isOnPress(std::string inputCode);
		bool isOnPress(std::vector<std::string> inputCodes);
		bool isOnPress(std::vector<std::string> inputCodes, PlayerID playerID);
		bool isOnDoublePress(std::string inputCode, unsigned int timeInterval);
		bool isOnDoublePress(std::vector<std::string> inputCodes, unsigned int timerInterval);
		bool isOnDoublePress(std::vector<std::string> inputCodes, unsigned int timerInterval, PlayerID playerID);
		bool isPressed(std::string inputCode);
		bool isPressed(std::vector<std::string> inputCodes);
		bool isPressed(std::vector<std::string> inputCodes, PlayerID playerID);
		bool isOnRelease(std::string inputCode);
		bool isOnRelease(std::vector<std::string> inputCodes);
		bool isOnRelease(std::vector<std::string> inputCodes, PlayerID playerID);
		bool isReleased(std::string inputCode);
		bool isReleased(std::string inputCode, PlayerID playerID);

		float getAxis(std::string inputCode);
		float getAxis(std::string inputCode, PlayerID playerID);

		bool isLeftMouseButtonOnPress();
		bool isLeftMouseButtonOnRelease();
		bool isLeftMouseButtonDoubleClicked();
		bool isMiddleMouseButtonOnPress();
		bool isMiddleMouseButtonOnRelease();
		bool isMiddleMouseButtonDoubleClicked();
		bool isRightMouseButtonOnPress();
		bool isRightMouseButtonOnRelease();
		bool isRightMouseButtonDoubleClicked();
		glm::vec2 getMousePosition();
		glm::vec2 getMouseWheelVelocity();
		glm::vec2 getMouseMotionVelocity();

		bool isDropFileDetected();
		std::string getDropFilePath();
		uint32_t getDropFileWindowID();

		void requestShutdown();
		void requestRestart();

		bool hasRequestedShutdown();
		bool hasRequestedRestart();
		bool hasRequestedCommandLine();
		bool getHasDetectedInputChanges();

		std::vector<SDL_Haptic*> getHaptics();
		std::unordered_map<PlayerID, SDL_JoystickID> getControllerPlayerMap();
		std::queue<PlayerID>& getNewlyLoadedPlayerIDs();
	private:
		void detectGameControllers();
		void pollInputEvents();

		std::unordered_map<std::string, SDL_Keycode> keyboardEventsByString;
		std::unordered_map<std::string, SDL_GameControllerButton> controllerButtonsByString;
		std::unordered_map<std::string, SDL_GameControllerAxis> controllerAxisEventsByString;

		std::set<std::string> heldInputs;

		std::set<SDL_Keycode> heldKeys;
		std::unordered_map<SDL_GameControllerButton, SDL_JoystickID> heldControllerButtons;

		std::unordered_map<SDL_GameControllerButton, SDL_ControllerButtonEvent> controllerButtonEvents;
		std::unordered_map<SDL_Keycode, SDL_KeyboardEvent> keyboardEvents;
		std::unordered_map<SDL_GameControllerAxis, SDL_ControllerAxisEvent> controllerAxisEvents;
		SDL_MouseButtonEvent mouseButtonEvent = { 0 };
		SDL_MouseMotionEvent mouseMotionEvent = { 0 };
		SDL_MouseWheelEvent  mouseWheelEvent = { 0 };
		SDL_WindowEvent windowEvent = { 0 };
		SDL_DropEvent dropEvent = { 0 };

		std::vector<SDL_GameController*> gameControllers;
		std::vector<SDL_Haptic*> haptics;

		std::unordered_map<PlayerID, SDL_JoystickID> controllerPlayerMap;
		std::queue<PlayerID> newlyLoadedPlayerIDs;

		bool hasDetectedInputChanges = false;
		bool shutdownRequest = false;
		bool restartRequest = false;
		void clear();
	};
}