#pragma once

#include "Exception.h"
class FullCapacityException : public Exception
{

public:
	FullCapacityException()
		:Exception()
	{

	}

	FullCapacityException(std::string message)
		:Exception(message)
	{

	}
};