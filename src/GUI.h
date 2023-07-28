#pragma once

#include <unordered_map>
#include "Controller.h"



#include <SFML/System.hpp>
#include <SFML/Window.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>

#include "actions/ActionList.h"
#include "objects/MenuViewer.h"
#include "objects/SortingViewer.h"
#include "objects/PathFindViewer.h"

class GUI
{
private:

	//variables
	int fps = 200;

	//window
	const int W_WIDTH = 1000;
	const int W_HEIGHT = 600;
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Vector2u lastWindowSize;

	//Camera view

	sf::View view;

	//events

	bool mouseClickEvent = false;
	bool mouseReleasedEvent = false;
	sf::Keyboard::Key keyPressed = sf::Keyboard::Key::Unknown;
	//controller

	SortType sortingType = SortType::CocktailSort;
	int numElements = 50;
	PathFindType pathFindType = PathFindType::Dijkstra;
	int numRows = 25;
	int numCols = 55;
	Controller controller;
	//simulator
	Simulation* simulation;
	SortingViewer* sortingViewer;

	void createSortingViewer();

	PathFindViewer* pathFindViewer;

	void createPathFindViewer();

	//menu
	MenuViewer* menuViewer;
	
	//program state
	
	enum class ProgramState { in_menu, in_sorting, in_pathfind};

	ProgramState state;

	//Mouse Positions

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	void updateMousePosition();

	//textures

	std::unordered_map<std::string, sf::Texture*> textures;

	void initTextures();
	void deleteTextures();

	//fonts
	
	sf::Font defaultFont;

	//Objects
	
	
	//private methods

	void initVariables();
	void initWindow();

	void eventsReset();

	//updates

	void updateMenu();
	void updateSortingViewer();
	void updatePathFindViewer();

public:
	GUI();			  //constructor
	virtual ~GUI();  //destructor
	//Accesors

	const bool running() const;

	sf::RenderWindow* getWindow();

	//functions


	void pollEvents();

	void update();

	void render();
};

