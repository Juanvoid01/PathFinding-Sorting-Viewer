#pragma once

#include "Action.h"

class MarkPathAction : public Action
{
public:
	MarkPathAction(int prim, int sec) {
		
		this->type = ActionType::MarkPath;
		this->prim = prim;
		this->sec = sec;

		this->miliSeconds = -1;
		this->red = -1;
		this->green = -1;
		this->blue = -1;

	}
};