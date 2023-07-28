#pragma once

#include "Action.h"

class DelayAction : public Action
{
public:
	DelayAction(int miliseconds) {

		this->type = ActionType::Delay;
		this->miliSeconds = miliSeconds;


		this->red = -1;
		this->green = -1;
		this->blue = -1;
		this->prim = -1;
		this->sec = -1;
	}

};

