#pragma once

#include "game/scene/scene.hpp"
#include "human_interface_devices.hpp"

namespace Lilliputian
{
	class UISystem
	{
	public:
		UISystem();
		void process(Scene& scene, HumanInterfaceDevices& hid);
	private:
	};
}

