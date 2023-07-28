#pragma once

#include "DelayAction.h"
#include "HighLightAction.h"
#include "SwapAction.h"
#include "NoneAction.h"
#include "MarkVisitedAction.h"
#include "MarkPathAction.h"
#include "AddWallAction.h"
#include "DeleteWallAction.h"
#include "ChangeValueAction.h"

class ActionList
{
private:

	const int defaultCapacity = 200;

	Action** list;
	int capacity;
	int numActions;
	int it;

	void checkCapacity();

public:

	ActionList();

	~ActionList();

	void forward();

	void backward();

	Action* getAction();

	void addAction(Action* action);

	int getNumActions();
};

