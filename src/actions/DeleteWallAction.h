#pragma once

#include "Action.h"

class DeleteWallAction : public Action
{
public:
	DeleteWallAction(int prim, int sec) {

		this->type = ActionType::DeleteWall;
		this->prim = prim;
		this->sec = sec;

		this->miliSeconds = -1;
		this->red = -1;
		this->green = -1;
		this->blue = -1;

	}
};