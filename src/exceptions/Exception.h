#pragma once
#include <string>
class Exception
{
protected:
	std::string message;
public:
	Exception() {
		this->message = "";
	}
	Exception(std::string message) {
		this->message = message;
	}

	std::string getMessage() {
		return this->message;
	}
};

