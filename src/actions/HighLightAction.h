#pragma once

#include "Action.h"

class HighLightAction : public Action
{
public:
	HighLightAction(int prim, int sec) {

		this->type = ActionType::HighLight;
		this->prim = prim;
		this->sec = sec;

		this->miliSeconds = -1;
		this->red = -1;
		this->green = -1;
		this->blue = -1;
		
	}
};

