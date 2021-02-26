#pragma once

#include "game/scene/scene_forest.hpp"
#include "human_interface_devices.hpp"

namespace Lilliputian
{
	class UISystem
	{
	public:
		UISystem();
		void process(SceneForest& scene, HumanInterfaceDevices& hid);
	private:
	};
}

