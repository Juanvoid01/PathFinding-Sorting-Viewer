#pragma once
#include "Exception.h"
class InvalidIndexException : public Exception
{

public:
	InvalidIndexException() 
		:Exception()
	{

	}

	InvalidIndexException(std::string message)
		:Exception(message)
	{

	}
};

