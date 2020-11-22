#pragma once

#include "../../Game/Scene/Scene.hpp"
#include "../../HumanInterfaceDevices.hpp"

namespace Lilliputian
{
	class UISystem
	{
	private:
	public:
		UISystem();
		void process(Scene& scene, HumanInterfaceDevices& hid);
	};
}

