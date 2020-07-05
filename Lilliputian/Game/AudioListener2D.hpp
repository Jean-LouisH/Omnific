#pragma once

#include "../Utilities/Aliases.hpp"

namespace Lilliputian
{
	class AudioListener2D
	{
	private:
		EntityID entityID;
	public:
		AudioListener2D(EntityID entityID);
		EntityID getEntityID();
	};
}