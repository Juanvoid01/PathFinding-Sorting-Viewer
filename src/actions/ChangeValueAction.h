#pragma once

#include "Action.h"

class ChangeValueAction : public Action
{
public:
	ChangeValueAction(int pos, int newValue, int lastValue) {

		this->type = ActionType::ChangeValue;
		this->prim = pos;
		this->newValue = newValue;
		this->lastValue = lastValue;
		this->sec = -1;
		this->red = -1;
		this->green = -1;
		this->blue = -1;
		this->miliSeconds = -1;
	}
};

