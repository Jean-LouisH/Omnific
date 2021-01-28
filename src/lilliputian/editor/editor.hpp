#pragma once

#include "../utilities/string.hpp"
#include "../game/scene/scene.hpp"
#include "../game/virtual_machine/virtual_machine.hpp"
#include "../os.hpp"

namespace Lilliputian
{
	class Editor
	{
	public:
		Editor(OS* os);
		Scene loadProjectList();
		Scene loadMainUI();
		void exportProject(String filepath);
	private:
		OS* os;
	};
}