#pragma once

#include <stdexcept>
#include <string>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>

namespace internal
{

template<typename T, size_t M, size_t N>
T createAssertException(char const (&fileName)[M], int line, char const (&functionName)[N], std::string const &message)
{
	return T(std::string(fileName) + ':' + std::to_string(line) + ' ' + functionName + ": " + message);
}

}

#define SIM_FAIL_M(msg) \
	throw ::internal::createAssertException<std::logic_error>(__FILE__, __LINE__, __FUNCTION__, msg)

#define SIM_ASSERT(cond) \
	if(!(cond)) \
		SIM_FAIL_M("Condition failed: " #cond)

#define SIM_ASSERT_M(cond, msg) \
	if(!(cond)) \
		SIM_FAIL_M(msg)

#define SIM_USERFAIL_M(msg) \
	throw ::internal::createAssertException<std::runtime_error>(__FILE__, __LINE__, __FUNCTION__, msg)

#define SIM_USERASSERT_M(cond, msg) \
	if(!(cond)) \
		SIM_USERFAIL_M(msg)

template<typename Target, typename Source>
inline Target checked_cast(Source const from)
{
	return boost::numeric_cast<Target>(from);
}

template<typename Target, typename Source>
inline void checked_cast(Target &to, Source const from)
{
	to = checked_cast<Target>(from);
}

template<typename T>
inline typename boost::make_unsigned<T>::type as_unsigned(T const from)
{
	return static_cast<typename boost::make_unsigned<T>::type>(from);
}

template<typename T>
inline typename boost::make_signed<T>::type as_signed(T const from)
{
	return static_cast<typename boost::make_signed<T>::type>(from);
}

template<typename T>
inline typename boost::make_unsigned<T>::type to_unsigned(T const from)
{
	return checked_cast<typename boost::make_unsigned<T>::type>(from);
}

template<typename T>
inline typename boost::make_signed<T>::type to_signed(T const from)
{
	return checked_cast<typename boost::make_signed<T>::type>(from);
}
