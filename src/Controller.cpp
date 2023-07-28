#include "Controller.h"


Controller::Controller() {
	list = nullptr;
	sim = nullptr;
	capacity = 0;
	numElems = 0;
}

Controller::~Controller() {
	if (list != nullptr)
	{
		delete [] list;
	}

	if (sim != nullptr)
	{
		delete sim;
	}
}

void  Controller::generateList(int newCapacity, int topLimit) {
	srand((unsigned int)time(NULL));

	if (list != nullptr)
	{
		delete [] list;
	}

	list = new int[newCapacity];
	numElems = 0;
	capacity = newCapacity;

	int n = 0;
	for (int i = 0; i < capacity; i++)
	{
		n = (rand() % (topLimit - 1)) + 1;
		list[i] = n;
		numElems++;
	}
}


void Controller::generateSimulation(int numElems, int topLimit, SortType sortType){

	if (sim != nullptr)
	{
		delete sim;
		sim = nullptr;
	}

	generateList(numElems, topLimit);

	sim = new Simulation(list, numElems);

	Sorting::Sort(list, numElems,sim, sortType);

}

Simulation* Controller::getSimulation() {
	return sim;
}

