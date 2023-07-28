#pragma once


#include "../actions/ActionList.h"

class Simulation
{
private:

	int* initialState;
	int numElems;


	ActionList* actionList;

public:
	Simulation(int* initialState, int numElems);

	~Simulation();

	void forward();

	void backward();

	Action* getAction();

	void addAction(Action* action);

	int* getInitialState();

	int getNumElems();
};

