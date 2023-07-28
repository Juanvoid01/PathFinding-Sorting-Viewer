#pragma once

#include "algorithms/Simulation.h"
#include "objects/Rectangle.h"
#include "objects/BallLineSelector.h"
#include "objects/Combobox.h"
#include <vector>
#include <unordered_map>
#include <string>

enum class SortingAction{Forward,Backward,PlayForward,PlayBackward,Pause, None};

class SortingViewer
{
private:

	
	Simulation* simulation;


	sf::Text* title;

	Button* backButton;
	Button* forwardButton;
	Button* backwardButton;
	Button* playForwardButton;
	Button* playBackwardButton;
	Button* resetButton;
	Button* pauseButton;

	sf::Text* numElementsDisplay;
	BallLineSelector* numElemsSelector;
	Combobox* sortSelector;
	sf::Text swapCounter;
	int numberOfSwaps = 0;

	std::vector<Rectangle*> objects;

	std::unordered_map<SortType,std::string> sortings;

	void initializeSortings(SortType sortType, sf::Font& font) {
		sortings[SortType::BubbleSort] = "BubbleSort";
		sortings[SortType::CocktailSort] = "CocktailSort";
		sortings[SortType::InsertionSort] = "InsertionSort";
		sortings[SortType::QuickSort] = "QuickSort";
		sortings[SortType::SelectionSort] = "SelectionSort";
		sortings[SortType::MergeSort] = "MergeSort";

		sortSelector->addElement(sortings[sortType]);
		for (auto& it : sortings) {
			if (it.second != sortings[sortType])
			{
				sortSelector->addElement(it.second);
			}
		}
	}

	void addObject(sf::Vector2f pos, sf::Vector2f size, std::string id) {

		this->objects.emplace_back(new Rectangle(pos, size, id));
	}

	void updateObjects() {
		for (auto it = objects.begin(); it != objects.end(); it++) {
			(*it)->update();
		}
	}

	void renderObjects(sf::RenderWindow* window) {
		for (auto it = objects.begin(); it != objects.end(); it++) {
			(*it)->render(window);
		}
	}

	void clearObjects() {
		for (auto it = objects.begin(); it != objects.end(); it++) {
			delete (*it);
		}
		this->objects.clear();
	}

	void initSimulation() {

		int* initialState = simulation->getInitialState();
		int numElems = simulation->getNumElems();

		sf::Vector2f pos = sf::Vector2f(38.f, 200.f);
		sf::Vector2f size = sf::Vector2f(10.f, 0.f);

		objects.reserve(numElems);
		for (int i = 0; i < numElems; i++)
		{
			pos.x += 12.f;
			size.y = initialState[i];
			addObject(pos, size, std::to_string(i));
		}

	}

	void excecuteAction(Action* action) {
		switch (action->type)
		{
		case ActionType::Swap: Swap(action->prim, action->sec);
			break;
		case ActionType::HighLight:ChangeHighLight(action->prim, action->sec);
			break;
		case ActionType::ChangeValue:ChangeValue(action->prim, action->newValue, action->lastValue);
			break;
		case ActionType::Delay:
			break;
		case ActionType::None:
			break;
		default:
			break;
		}
	}

	void Swap(int prim, int sec) {

		sf::Vector2f pos_aux = sf::Vector2f(objects[prim]->getPosition());
		objects[prim]->setPosition(objects[sec]->getPosition());
		objects[sec]->setPosition(pos_aux);

		std::swap(objects[prim], objects[sec]);

		if (this->forwardAction || this->playForwardAction)
		{
			swapCounter.setString("Swaps : " + std::to_string(++numberOfSwaps));
		}
		else
		{
			swapCounter.setString("Swaps : " + std::to_string(--numberOfSwaps));
		}
		
	}

	void ChangeValue(int pos, int newValue, int lastValue) {

		if (this->forwardAction || this->playForwardAction)
		{
			objects[pos]->setSize(sf::Vector2f(objects[pos]->getSize().x, newValue));
			swapCounter.setString("Swaps : " + std::to_string(++numberOfSwaps));
		}
		else
		{
			objects[pos]->setSize(sf::Vector2f(objects[pos]->getSize().x, lastValue));
			swapCounter.setString("Swaps : " + std::to_string(--numberOfSwaps));
		}
	}

	void ChangeHighLight(int prim, int sec) {
		if (objects[prim]->getColor() == sf::Color(0, 200, 0))
		{
			objects[prim]->setColor(sf::Color(200, 0, 0));
			objects[sec]->setColor(sf::Color(200, 0, 0));
		}
		else if (objects[prim]->getColor() == sf::Color(200, 0, 0))
		{
			objects[prim]->setColor(sf::Color(0, 200, 0));
			objects[sec]->setColor(sf::Color(0, 200, 0));
		}

	}

	void resetActions() {
		exitAction = false;
		resetAction = false;
		playForwardAction = false;
		playBackwardAction = false;
		forwardAction = false;
		backwardAction = false;
		pauseAction = false;
	}

public:

	bool exitAction;
	bool resetAction;
	bool playForwardAction;
	bool playBackwardAction;
	bool forwardAction;
	bool backwardAction;
	bool mousePressedAction;
	bool pauseAction;

	SortingViewer(Simulation* sim,std::unordered_map<std::string, sf::Texture*>& textures,sf::Font& font,int numElems,SortType sortType) {
		simulation = sim;
		initSimulation();


		title = new sf::Text("SORTING",font,25);
		title->setPosition(450.0f, 10.0f);
		title->setFillColor(sf::Color::Black);
		//title->setOutlineColor(sf::Color(200, 200, 255));
		title->setOutlineThickness(1);

		backButton = new Button(sf::Vector2f(10.f,10.f), "BackButton", textures["BackButton"]);

		sf::Vector2f iniPos = sf::Vector2f(50.f, 80.f);

		playBackwardButton = new Button(iniPos, "PlayBackwardButton", textures["PlayBackwardButton"]);
		iniPos = sf::Vector2f(iniPos.x + 70.f, iniPos.y);
		backwardButton = new Button(iniPos, "BackwardButton", textures["BackwardButton"]);
		iniPos = sf::Vector2f(iniPos.x + 70.f, iniPos.y);
		pauseButton = new Button(iniPos, "PauseButton", textures["PauseButton"]);
		iniPos = sf::Vector2f(iniPos.x + 70.f, iniPos.y);
		forwardButton = new Button(iniPos, "ForwardButton", textures["ForwardButton"]);
		iniPos = sf::Vector2f(iniPos.x + 70.f, iniPos.y);
		playForwardButton = new Button(iniPos, "PlayForwardButton", textures["PlayForwardButton"]);

		resetButton = new Button(sf::Vector2f(750.f, 80.f), "ResetButton", textures["ResetButton"]);
		
		backButton->setScale(sf::Vector2f(0.3f,0.3f));

		forwardButton->setScale(sf::Vector2f(0.2f, 0.2f));
		backwardButton->setScale(sf::Vector2f(0.2f, 0.2f));
		pauseButton->setScale(sf::Vector2f(0.2f, 0.2f));
		resetButton->setScale(sf::Vector2f(0.15f, 0.15f));
		playForwardButton->setScale(sf::Vector2f(0.2f, 0.2f));
		playBackwardButton->setScale(sf::Vector2f(0.2f, 0.2f));

		
		numElemsSelector = new BallLineSelector(sf::Vector2f(600.f,95.f), sf::Vector2f(100.f, 5.f),5,75);
		numElemsSelector->setValue(numElems);

		numElementsDisplay = new sf::Text(std::to_string(numElemsSelector->getValue()), font, 15);
		numElementsDisplay->setPosition(numElemsSelector->getPosition());
		numElementsDisplay->move((numElemsSelector->getSize().x/2.f)-7.5f,-25.f);
		numElementsDisplay->setFillColor(sf::Color::Black);

		sortSelector = new Combobox(sf::Vector2f(850.f,90.f),&font);
		initializeSortings(sortType,font);

		swapCounter.setPosition(sf::Vector2f(40.f, 550.f));
		swapCounter.setFont(font);
		swapCounter.setCharacterSize(11.f);
		//swapCounter.setOutlineThickness(0.5f);
		swapCounter.setString("Swaps : 0");
		swapCounter.setFillColor(sf::Color::Black);
		resetActions();
	}

	~SortingViewer() {
		clearObjects();
		delete backButton;
		delete forwardButton;
		delete backwardButton;
		delete playForwardButton;
		delete playBackwardButton;
		delete title;
		delete resetButton;
		delete pauseButton;
		delete numElemsSelector;
		delete numElementsDisplay;
		delete sortSelector;
	}


	void executeForwardAction() {

		simulation->forward();
		excecuteAction(simulation->getAction());

	}


	void executeBackwardAction() {

		excecuteAction(simulation->getAction());
		simulation->backward();

	}



	void update(SortingAction action) {
		switch (action)
		{
		case SortingAction::Forward:executeForwardAction();
			forwardAction = false;
			break;
		case SortingAction::Backward:executeBackwardAction();
			backwardAction = false;
			break;
		case SortingAction::PlayForward:executeForwardAction();
			playForwardAction = simulation->getAction()->type == ActionType::None ? false : true;
			break;
		case SortingAction::PlayBackward:executeBackwardAction();
			playBackwardAction = simulation->getAction()->type == ActionType::None ? false : true;
			break;
		case SortingAction::Pause:resetActions();
			break;
		default:
			break;
		}

	}

	void update(bool mousePressed, bool mouseReleased, sf::Vector2f mousePos) {

		if (mousePressed)
		{
			mousePressedAction = true;

			if (backButton->isSelected(mousePos))
			{
				resetActions();
				exitAction = true;
			}
			else if (forwardButton->isSelected(mousePos))
			{
				resetActions();
				forwardAction = true;
			}
			else if (backwardButton->isSelected(mousePos))
			{
				resetActions();
				backwardAction = true;
			}
			else if (playForwardButton->isSelected(mousePos))
			{
				resetActions();
				playForwardAction = true;
			}
			else if (playBackwardButton->isSelected(mousePos))
			{
				resetActions();
				playBackwardAction = true;
			}
			else if (resetButton->isSelected(mousePos))
			{
				resetActions();
				resetAction = true;
			}
			else if (pauseButton->isSelected(mousePos))
			{
				resetActions();
				pauseAction = true;
			}
		}

		pauseButton->update(mousePos);
		resetButton->update(mousePos);
		playBackwardButton->update(mousePos);
		playForwardButton->update(mousePos);
		backwardButton->update(mousePos);
		forwardButton->update(mousePos);
		backButton->update(mousePos);

		std::string lastSortType = sortSelector->getValue();
		sortSelector->update(mousePressed, mousePos);
		if (lastSortType!= sortSelector->getValue())
		{
			resetAction = true;
		}

		numElemsSelector->update(mousePressed, mouseReleased, mousePos);
		if (mouseReleased)
		{
			std::string lastValue = numElementsDisplay->getString();
			std::string newValue = std::to_string(numElemsSelector->getValue());
			if (lastValue != newValue)
			{
				numElementsDisplay->setString(newValue);
				resetAction = true;
			}
			
			mousePressedAction = false;
		}
	}

	void render(sf::RenderWindow* window) {
		renderObjects(window);

		window->draw(*title);

		backButton->render(window);
		forwardButton->render(window);
		backwardButton->render(window);
		resetButton->render(window);
		pauseButton->render(window);
		playForwardButton->render(window);
		playBackwardButton->render(window);
		numElemsSelector->render(window);
		sortSelector->render(window);
		window->draw(*numElementsDisplay);
		window->draw(swapCounter);
	}

	int getNumElements() {
		return numElemsSelector->getValue();
	}

	SortType getSortType() {
		for (auto& it : sortings) {
			if (it.second == sortSelector->getValue())
			{
				return it.first;
			}
		}
		return SortType::BubbleSort;
	}

};

