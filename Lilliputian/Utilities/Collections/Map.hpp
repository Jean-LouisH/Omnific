#pragma once

#include <map>
#include <unordered_map>

namespace Lilliputian
{
	template <class _Kty, class _Ty>
	using Pair = std::pair<_Kty, _Ty>;

	template <class _Kty, class _Ty>
	using Map = std::map<const _Kty, _Ty>;

	template <class _Kty, class _Ty>
	using UnorderedMap = std::unordered_map<const _Kty, _Ty>;

	template <class _Kty, class _Ty>
	using Multimap = std::multimap<const _Kty, _Ty>;

	template <class _Kty, class _Ty>
	using UnorderedMultimap = std::unordered_multimap<const _Kty, _Ty>;
}
