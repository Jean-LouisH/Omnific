#pragma once

#include <map>

namespace Lilliputian
{
	template <class _Kty, class _Ty>
	using Pair = std::pair<_Kty, _Ty>;

	template <class _Kty, class _Ty>
	using Map = std::map<const _Kty, _Ty>;

	template <class _Kty, class _Ty>
	using Multimap = std::multimap<const _Kty, _Ty>;
}
