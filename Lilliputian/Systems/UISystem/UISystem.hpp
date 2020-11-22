#pragma once

#include "../../Game/Scene/Scene.hpp"
#include "../../HumanInterfaceDevices.hpp"

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

