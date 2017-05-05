#pragma once

#include <stdexcept>
#include <string>

template<typename T, size_t M, size_t N>
T createAssertException(char const (&fileName)[M], int line, char const (&functionName)[N], std::string const &message)
{
	return T(std::string(fileName) + ':' + std::to_string(line) + ' ' + functionName + ": " + message);
}

#define SIM_FAIL_M(msg) \
	throw createAssertException<std::logic_error>(__FILE__, __LINE__, __FUNCTION__, msg)

#define SIM_ASSERT(cond) \
	if(!(cond)) \
		SIM_FAIL_M("Condition failed: " #cond)

#define SIM_ASSERT_M(cond, msg) \
	if(!(cond)) \
		SIM_FAIL_M(msg)

#define SIM_USERFAIL_M(msg) \
	throw createAssertException<std::runtime_error>(__FILE__, __LINE__, __FUNCTION__, msg)

#define SIM_USERASSERT_M(cond, msg) \
	if(!(cond)) \
		SIM_USERFAIL_M(msg)
