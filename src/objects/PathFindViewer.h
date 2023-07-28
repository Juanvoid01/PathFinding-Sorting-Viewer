#pragma once

#include "algorithms/Simulation.h"
#include "Objects/Combobox.h"
#include "grid.h"
#include "MazeGenerator.h"
#include <vector>
#include <unordered_map>
#include <string>

enum class PathFindAction { Forward, Backward, PlayForward, PlayBackward, Pause, None };
enum class PathFindState {Editing, Running, Mazing};

class PathFindViewer {

private:
	Simulation* simulation;
	PathFinding pf;
	PathFindState state = PathFindState::Editing;
	ActionList* actionList;
	sf::Text* title;

	Button* backButton;
	Button* forwardButton;
	Button* backwardButton;
	Button* playForwardButton;
	Button* playBackwardButton;
	Button* resetButton;
	Button* pauseButton;

	MazeGenerator* mazeGenerator;

	Combobox* pathFindSelector;

	sf::Text visitedCounter;
	int numberOfvisited = 0;

	std::unordered_map<PathFindType, std::string> pathFindAlgorithms;
	std::unordered_map<std::string, bool> weighteds;

	Grid* grid;

	void initializePathFindAlgorithms(PathFindType pathFindType, const sf::Font& font) {

		pathFindAlgorithms[PathFindType::Dijkstra] = "Dijkstra";
		pathFindAlgorithms[PathFindType::AStar] = "AStar";
		pathFindAlgorithms[PathFindType::DepthFirst] = "DepthFirst";
		pathFindAlgorithms[PathFindType::BreadthFirst] = "BreadthFirst"; 
		pathFindAlgorithms[PathFindType::Bidirectional] = "Bidirectional";

		weighteds["Dijkstra"] = true;
		weighteds["AStar"] = true;
		weighteds["DepthFirst"] = false;
		weighteds["BreadthFirst"] = false;
		weighteds["Bidirectional"] = false;


		pathFindSelector->addElement(pathFindAlgorithms[pathFindType]);
		for (auto& it : pathFindAlgorithms) {
			if (it.second != pathFindAlgorithms[pathFindType])
			{
				pathFindSelector->addElement(it.second);
			}
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
		mousePressedAction = false;
	}

	void excecuteAction(Action* action) {
		switch (action->type)
		{
		case ActionType::MarkVisited: markVisited(action->prim, action->sec);
			break;
		case ActionType::MarkPath:markPath(action->prim, action->sec);
			break;
		case ActionType::AddWall:addWall(action->prim, action->sec);
			break;
		case ActionType::DeleteWall:deleteWall(action->prim, action->sec);
			break;
		case ActionType::Delay:
			break;
		case ActionType::None:
			break;
		default:
			break;
		}
	}

	void markVisited(int row, int col) {

		grid->changeVisited(sf::Vector2i(row,col));

		if (this->forwardAction || this->playForwardAction)
		{
			visitedCounter.setString("Visited Nodes : " + std::to_string(++numberOfvisited));
		}
		else
		{
			visitedCounter.setString("Visited Nodes : " + std::to_string(--numberOfvisited));
		}
	}

	void markPath(int row, int col) {
		grid->changePath(sf::Vector2i(row, col));
	}

	void addWall(int row, int col) {
		grid->setWall(sf::Vector2i(row, col), true);
	}

	void deleteWall(int row, int col) {
		grid->setWall(sf::Vector2i(row, col), false);
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

	PathFindViewer(Simulation* sim, std::unordered_map<std::string, sf::Texture*>& textures, sf::Font& font, PathFindType pathFindType) {
		simulation = sim;
		actionList = new ActionList();
		title = new sf::Text("PATHFIND", font, 30);
		title->setPosition(430.0f, 10.0f);
		title->setFillColor(sf::Color::Black);
		//title->setOutlineColor(sf::Color(200, 200, 255));
		title->setOutlineThickness(1);

		backButton = new Button(sf::Vector2f(10.f, 10.f), "BackButton", textures["BackButton"]);

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

		resetButton = new Button(sf::Vector2f(815.f, 87.f), "ResetButton", textures["ResetButton"]);

		backButton->setScale(sf::Vector2f(0.3f, 0.3f));

		forwardButton->setScale(sf::Vector2f(0.2f, 0.2f));
		backwardButton->setScale(sf::Vector2f(0.2f, 0.2f));
		pauseButton->setScale(sf::Vector2f(0.2f, 0.2f));
		resetButton->setScale(sf::Vector2f(0.15f, 0.15f));
		playForwardButton->setScale(sf::Vector2f(0.2f, 0.2f));
		playBackwardButton->setScale(sf::Vector2f(0.2f, 0.2f));



		pathFindSelector = new Combobox(sf::Vector2f(880.f, 95.f), &font);
		initializePathFindAlgorithms(pathFindType, font);

		mazeGenerator = new MazeGenerator(sf::Vector2f(680.f, 92.f), font);

		grid = new Grid(sf::Vector2f(50.f,160.f), 16.f,25,55);

		visitedCounter.setPosition(sf::Vector2f(50.f, 570.f));
		visitedCounter.setFont(font);
		visitedCounter.setCharacterSize(11.f);
		//visitedCounter.setOutlineThickness(0.5f);
		visitedCounter.setString("Visited Nodes : 0");
		visitedCounter.setFillColor(sf::Color::Black);

		resetActions();
	}

	~PathFindViewer() {

		delete title;
		delete backButton;
		

		delete forwardButton;
		delete backwardButton;
		delete playForwardButton;
		delete playBackwardButton;
		delete resetButton;
		delete pauseButton;
		delete grid;
		delete mazeGenerator;
		delete actionList;
	}

	void executeForwardAction() {

		actionList->forward();
		excecuteAction(actionList->getAction());

	}


	void executeBackwardAction() {

		excecuteAction(actionList->getAction());
		actionList->backward();

	}

	PathFindType getPathFindType() {
		for (auto& it : pathFindAlgorithms) {
			if (it.second == pathFindSelector->getValue())
			{
				return it.first;
			}
		}
		return PathFindType::Dijkstra;
	}

	int getNumRows() {
		return grid->getNumRows();
	}

	int getNumCols() {
		return grid->getNumCols();
	}
	void update(PathFindAction action) {
		switch (action)
		{
		case PathFindAction::Forward:executeForwardAction();
			forwardAction = false;
			break;
		case PathFindAction::Backward:executeBackwardAction();
			backwardAction = false;
			break;
		case PathFindAction::PlayForward:executeForwardAction();
			playForwardAction = actionList->getAction()->type == ActionType::None ? false : true;
			if (!playForwardAction && state == PathFindState::Mazing)
			{
				resetActionList();
				state = PathFindState::Editing;
			}
			break;
		case PathFindAction::PlayBackward:executeBackwardAction();
			playBackwardAction = actionList->getAction()->type == ActionType::None ? false : true;
			break;
		case PathFindAction::Pause:resetActions();
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
				
				if (state == PathFindState::Editing)
				{
					resetActions();
					resetActionList();
					pf.PathFind(grid, actionList, getPathFindType());
					state = PathFindState::Running;
					forwardAction = true;
				}
				else if (state == PathFindState::Running)
				{
					resetActions();
					forwardAction = true;
				}

			}
			else if (state == PathFindState::Running && backwardButton->isSelected(mousePos))
			{
				resetActions();
				backwardAction = true;
			}
			else if (playForwardButton->isSelected(mousePos))
			{
				if (state == PathFindState::Editing)
				{
					resetActions();
					resetActionList();
					pf.PathFind(grid, actionList, getPathFindType());
					state = PathFindState::Running;
					playForwardAction = true;
				}
				else if (state == PathFindState::Running)
				{
					resetActions();
					playForwardAction = true;
				}
				
				
			}
			else if (state == PathFindState::Running && playBackwardButton->isSelected(mousePos))
			{
				resetActions();
				playBackwardAction = true;
			}
			else if (resetButton->isSelected(mousePos))
			{
				resetActions();
				resetAction = true;
			}
			else if (state == PathFindState::Running && pauseButton->isSelected(mousePos))
			{
				resetActions();
				pauseAction = true;
			}
			else if (mazeGenerator->isSelected(mousePos))
			{
				resetActions();
				resetActionList();
				state = PathFindState::Mazing;
				playForwardAction = true;
			}
		}
		pauseButton->update(mousePos);
		resetButton->update(mousePos);
		playBackwardButton->update(mousePos);
		playForwardButton->update(mousePos);
		backwardButton->update(mousePos);
		forwardButton->update(mousePos);
		backButton->update(mousePos);

		mazeGenerator->update(mousePressed, mousePos, grid, actionList);


		bool changeMade = false;

		grid->update(mousePressed, mouseReleased, mousePos, changeMade);

		if (changeMade)
		{
			resetActions();
			resetActionList();
			grid->resetVisitedNodes();
			state = PathFindState::Editing;
		}
		

		std::string lastPathFindType = pathFindSelector->getValue();
		pathFindSelector->update(mousePressed, mousePos);
		if (lastPathFindType != pathFindSelector->getValue())
		{
			resetActions();
			resetActionList();
			grid->resetVisitedNodes();
			state = PathFindState::Editing;

			//enable or disable the weights
			grid->setEnableWeights(weighteds[pathFindSelector->getValue()]);

		}


		if (mouseReleased) {
			mousePressedAction = false;
		}
	}

	void render(sf::RenderWindow* window) {
		window->draw(*title);
		backButton->render(window);
		forwardButton->render(window);
		backwardButton->render(window);
		resetButton->render(window);
		pauseButton->render(window);
		playForwardButton->render(window);
		playBackwardButton->render(window);
		grid->render(window);
		pathFindSelector->render(window);
		mazeGenerator->render(window);
		window->draw(visitedCounter);
	}

	void resetActionList() {
		numberOfvisited = 0;
		visitedCounter.setString("Visited Nodes : 0");
		delete actionList;
		actionList = new ActionList();
	}
};