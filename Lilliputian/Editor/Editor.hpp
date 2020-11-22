#pragma once

#include "../Utilities/String.hpp"
#include "../Game/Scene/Scene.hpp"
#include "../Game/VirtualMachine/VirtualMachine.hpp"
#include "../OS.hpp"

namespace Lilliputian
{
	class Editor
	{
	private:
		OS* os;
	public:
		Editor(OS* os);
		Scene loadProjectList();
		Scene loadMainUI();
		void exportProject(String filepath);
	};
}