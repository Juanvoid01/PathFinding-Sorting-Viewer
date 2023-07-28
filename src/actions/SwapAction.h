#pragma once

#include "Action.h"

class SwapAction : public Action
{
public:
	SwapAction(int prim, int sec) {

		this->type = ActionType::Swap;
		this->prim = prim;
		this->sec = sec;

		this->red = -1;
		this->green = -1;
		this->blue = -1;
		this->miliSeconds = -1;
	}
};

