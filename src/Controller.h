#pragma once

#include "algorithms/Sorting.h"
#include "algorithms/PathFinding.h"
#include "algorithms/Simulation.h"
#include "exceptions/FullCapacityException.h"

#include<stdlib.h>
#include <iostream>
#include<time.h>


class Controller
{
private:
	const int defaultCapacity = 20;
	int* list;
	int numElems;
	int capacity;

	Simulation* sim;

	void generateList(int newCapacity, int topLimit);
public:
	Controller();
	~Controller();

	void generateSimulation(int numElems, int topLimit, SortType sortType);

	Simulation* getSimulation();

	void printList() {
		for (int i = 0; i < numElems; i++)
		{
			std::cout << list[i] << ",";
		}
		std::cout << std::endl;
	}
};

