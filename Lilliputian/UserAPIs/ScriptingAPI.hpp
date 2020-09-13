#pragma once

#include "EngineAPI.hpp"
#include "GameAPI.hpp"
#include "InputAPI.hpp"
#include "SceneAPI.hpp"

namespace Lilliputian
{
	class ScriptingAPI
	{
	private:
		EngineAPI* engineAPI;
		GameAPI* gameAPI;
		InputAPI* inputAPI;
		SceneAPI* sceneAPI;
	public:
		ScriptingAPI(
			EngineAPI* engineAPI,
			GameAPI* gameAPI, 
			InputAPI* inputAPI,
			SceneAPI* sceneAPI
		);
		EngineAPI& engine() const;
		GameAPI& game() const;
		InputAPI& input() const;
		SceneAPI& scene() const;
	};
}

