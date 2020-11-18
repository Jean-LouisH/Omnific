#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"

namespace Lilliputian
{
	class UICircularItemList
	{
	private:
		uint32_t lowerBound = 0;
		uint32_t upperBound;
		uint32_t index = 0;
	public:
		void setLowerBound(uint32_t lowerBound);
		void setUpperBound(uint32_t upperBound);
		void increment();
		void decrement();
		void setIndex(uint32_t index);
		uint32_t getIndex();
	};
}