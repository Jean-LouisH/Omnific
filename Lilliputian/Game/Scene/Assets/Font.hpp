#pragma once

#include "Asset.hpp"

namespace Lilliputian
{
	class Font : public Asset
	{
	public:
		Font(const char* filepath);
		virtual void unload();
	private:
	};
}