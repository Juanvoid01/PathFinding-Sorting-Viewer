#pragma once

enum class ActionType{Swap,ChangeValue,HighLight,Delay,None,MarkVisited,MarkPath,AddWall,DeleteWall};

class Action
{
public:
	ActionType type;
	int prim;
	int sec;
	int red;
	int green;
	int blue;
	int miliSeconds;
	int newValue = -1;
	int lastValue = -1;

	Action() {
		type = ActionType::None;
		prim = -1;
		sec = -1;
		red = -1;
		green = -1;
		blue = -1;
		miliSeconds = -1;
	}
};

