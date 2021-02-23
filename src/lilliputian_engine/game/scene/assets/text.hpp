#pragma once

#include "asset.hpp"

namespace Lilliputian
{
	class Text : public Asset
	{
	public:
		Text(const char* filepath);
		virtual void unload();
	private:
		char* data = nullptr;
	};
}