#pragma once

#include "Action.h"

class NoneAction : public Action
{
public:
	NoneAction() {

		this->type = ActionType::None;
		this->prim = -1;
		this->sec = -1;
		this->red = -1;
		this->green = -1;
		this->blue = -1;
		this->miliSeconds = -1;
	}
};