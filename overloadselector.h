#pragma once

template<typename _SignatureT> struct overload { };
template<typename _ReturnT, typename... _ArgsT>
struct overload<_ReturnT(_ArgsT...)>
{
	template<typename _ClassT>
	inline constexpr static auto get(_ReturnT(_ClassT::*ptr)(_ArgsT...)){ return ptr; }
};
