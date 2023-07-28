#include "ActionList.h"


ActionList::ActionList() {

	list = new Action*[defaultCapacity];
	for (int i = 0; i < defaultCapacity; i++)
	{
		list[i] = nullptr;
	}
	numActions = 0;
	capacity = defaultCapacity;
	it = 0;

}

ActionList::~ActionList() {

	for (int i = 0; i < capacity; i++)
	{
		if (list[i]!=nullptr)
		{
			delete list[i];
		}
	}

	delete[] list;
}

void ActionList::checkCapacity() {

	if (numActions == capacity)
	{
		capacity += defaultCapacity;

		Action** newActionList = new Action*[capacity];

		for (int i = 0; i < numActions; i++)
		{
			newActionList[i] = list[i];
		}

		for (int i = numActions; i < capacity; i++)
		{
			newActionList[i] = nullptr;
		}

		delete[] list;

		list = newActionList;
		
	}

}

void ActionList::forward() {
	if (it < numActions - 1)
	{
		it++;
	}
}

void ActionList::backward() {
	if (it > 0)
	{
		it--;
	}
}

Action* ActionList::getAction() {
	return list[it];
}

void ActionList::addAction(Action* action) {
	checkCapacity();
	list[numActions++] = action;
}

int ActionList::getNumActions() {
	return this->numActions;
}
