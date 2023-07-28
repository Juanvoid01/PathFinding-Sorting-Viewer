#pragma once

#include <SFML/Graphics.hpp>
#include "../actions/ActionList.h"
#include "Grid.h"
#include <random>

class MazeGenerator {

public:

	MazeGenerator(sf::Vector2f pos, const sf::Font& font) {
		position = pos;
		size = sf::Vector2f(110.f, 30.f);

		box.setPosition(pos);
		box.setSize(size);
		box.setFillColor(sf::Color(200, 200, 200));
		box.setOutlineThickness(4.f);
		box.setOutlineColor(sf::Color::Black);

		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setPosition(pos.x + 2,pos.y + 5);
		text.setCharacterSize(size.y / 2.f);
		text.setString("Generate Maze");

	}

	void generate(Grid* grid, ActionList* actionList) {

		grid->resetNodes();

		actionList->addAction(new NoneAction());
		recursiveDivision(grid, actionList);
		actionList->addAction(new NoneAction());
	}

	bool isSelected(sf::Vector2f p) {
		sf::Vector2f endPos = sf::Vector2f(position.x + size.x, position.y + size.y);
		return p.x >= position.x && p.x <= endPos.x && p.y >= position.y && p.y <= endPos.y;
	}


	void update(bool mousePressed, sf::Vector2f mousePos, Grid* grid, ActionList* actionList) {
		if (selected) {

			if (mousePressed) {
				generate(grid, actionList);
			}

			if (!isSelected(mousePos)) {
				selected = false;
				box.setOutlineColor(sf::Color::Black);
			}
		}
		else
		{
			if (isSelected(mousePos)) {
				selected = true;
				box.setOutlineColor(sf::Color::Red);
			}
		}
	}

	void render(sf::RenderWindow* window) {
		window->draw(box);
		window->draw(text);
	}

private:
	
	sf::Vector2f position;
	sf::Vector2f size;
	sf::RectangleShape box;
	sf::Text text;
	bool selected = false;
	std::random_device random_device;

	
	void createBorderWalls(int x, int y, int width,int height,ActionList* actionList) {

		for (int i = x; i < width; i++)
		{
			actionList->addAction(new AddWallAction(x, i));
			actionList->addAction(new AddWallAction(height, i));
		}

		for (int i = y; i < height; i++)
		{
			actionList->addAction(new AddWallAction(i, x));
			actionList->addAction(new AddWallAction(i, width));
		}
		actionList->addAction(new AddWallAction(height, width));
	}

	void divide(int x, int y, int width, int height, bool** nodes, ActionList* actionList) {

		std::mt19937 random_generator(random_device());

		char orientation;
		int newWall, newHole, newHeight, newWidth;
		int xPair, yPair, newHeightPair, newWidthPair;

		//decide orientation

		if (width < height)
		{
			orientation = 'h'; //horizontal
		}
		else if (width > height)
		{
			orientation = 'v'; //vertical
		}
		else
		{
			std::uniform_int_distribution<int> random_dir(0, 1);
			orientation = random_dir(random_generator) == 0 ? 'h' : 'v';
		}

		if (orientation == 'h')
		{
			if (height < 5 || width < 3)
			{
				return;
			}

			//random place the wall and the hole

			std::uniform_int_distribution<int> random_wall(2, height - 3);
			std::uniform_int_distribution<int> random_hole(1, width - 2);

			//wall will be on even position and hole on odd position

			newWall = y + random_wall(random_generator);
			newWall % 2 == 0 ? 0 : newWall--;

			newHole = x + random_hole(random_generator);
			newHole % 2 == 0 ? newHole++ : 0;

			//place walls and hole

			for (int i = x; i < x + width - 1; i++)
			{
				if (i != newHole)
				{
					nodes[newWall][i] = true;
					actionList->addAction(new AddWallAction(newWall, i));
				}
			}

			//calculate newValues

			newHeight = newWall - y + 1;
			newWidth = width;

			yPair = newWall;
			xPair = x;
			newHeightPair = y + height - newWall;
			newWidthPair = width;

		}
		else //orientation =='v'
		{

			if (width < 5 || height < 3)
			{
				return;
			}


			//random place the wall and the hole

			std::uniform_int_distribution<int> random_wall(2, width - 3);
			std::uniform_int_distribution<int> random_hole(1, height - 2);

			//wall will be on even position and hole on odd position

			newWall = x + random_wall(random_generator);
			newWall % 2 == 0 ? 0 : newWall--;

			newHole = y + random_hole(random_generator);
			newHole % 2 == 0 ? newHole++ : 0;

			//place walls and hole

			for (int i = y; i < y + height - 1; i++)
			{
				if (i != newHole)
				{
					nodes[i][newWall] = true;
					actionList->addAction(new AddWallAction(i, newWall));
				}
			}

			//calculate newValues

			newHeight = height;
			newWidth = newWall - x + 1;

			yPair = y;
			xPair = newWall;
			newHeightPair = height;
			newWidthPair = x + width - newWall;
		}

		//recursive calls

		divide(x, y, newWidth, newHeight, nodes, actionList);

		divide(xPair, yPair, newWidthPair, newHeightPair, nodes, actionList);
	}

	void recursiveDivision(Grid* grid, ActionList* actionList) {

		bool** nodes; //array of nodes, true if the node is a wall
		int height = grid->getNumRows() - 1;
		int width = grid->getNumCols() - 1;

		nodes = new bool* [height];
		for (int i = 0; i < height; i++) {
			nodes[i] = new bool[width];
			for (int j = 0; j < width; j++)
			{
				nodes[i][j] = false;
			}
		}

		//create border walls
		createBorderWalls(0, 0, width, height, actionList);


		divide(1, 1, width, height, nodes, actionList);


		//free memory
		for (int i = 0; i < height; i++) {
			delete[] nodes[i];
		}
		delete[] nodes;
	}
};