#include "Simulation.h"


Simulation::Simulation(int* initialState, int numElems) {
	this->initialState = new int[numElems];
	for (int i = 0; i < numElems; i++)
	{
		this->initialState[i] = initialState[i];
	}
	this->numElems = numElems;

	actionList = new ActionList();
}

Simulation::~Simulation() {
	delete [] initialState;
	delete actionList;
}

void Simulation::forward() {
	actionList->forward();
}

void Simulation::backward() {
	actionList->backward();
}

Action* Simulation::getAction() {
	return actionList->getAction();
}

void Simulation::addAction(Action* action) {
	actionList->addAction(action);
	
}

int* Simulation::getInitialState() {
	return initialState;
}

int Simulation::getNumElems() {
	return numElems;
}