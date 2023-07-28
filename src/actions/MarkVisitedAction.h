#pragma once

#include "Action.h"

class MarkVisitedAction : public Action
{
public:
	MarkVisitedAction(int prim, int sec) {

		this->type = ActionType::MarkVisited;
		this->prim = prim;
		this->sec = sec;

		this->miliSeconds = -1;
		this->red = -1;
		this->green = -1;
		this->blue = -1;

	}
};