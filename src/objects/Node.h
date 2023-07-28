#pragma once
#include <SFML/Graphics.hpp>



class Node {

private:
	

	sf::RectangleShape hitbox;

	int weight;
	bool visited;
	bool isWall;
	bool isStart;
	bool isEnd;
	bool isPath;

	sf::Vector2f size;
	sf::Vector2f pos;

	sf::Color wallColor = sf::Color(256,256,256);
	sf::Color unvisitedColor = sf::Color(150,150,150);
	sf::Color borderColor = sf::Color(200, 200, 200);
	sf::Color pathColor = sf::Color(212, 175, 55);
	sf::Color visitedColor = sf::Color(176,216,230);

public:

	Node() {
		weight = 0;
		visited = false;
		isWall = false;
		isStart = false;
		isEnd = false;
		isPath = false;
	}

	Node(sf::Vector2f pos, sf::Vector2f size, int weight = 0,
		sf::Color color = sf::Color(150,150,150)) {
		this->weight = weight;
		hitbox.setPosition(pos);
		hitbox.setSize(size);
		hitbox.setFillColor(color);
		hitbox.setOutlineThickness(size.x / 10.f);
		hitbox.setOutlineColor(borderColor);
		visited = false;
		isWall = false;
		isStart = false;
		isEnd = false;
		isPath = false;
		this->pos = pos;
		this->size = size;
	}

	void update() {

	}

	void setAsStart() {
		if (!isStart) {
			isStart = true;
			
			visited = false;
			isWall = false;
			isEnd = false;
			isPath = false;
			weight = 0;

			hitbox.setFillColor(unvisitedColor);
		}

	}

	void deleteAsStart() {
		if (isStart) {
			isStart = false;
			hitbox.setFillColor(unvisitedColor);
		}

	}

	void setAsEnd() {
		if (!isEnd) {
			isEnd = true;
			
			visited = false;
			isWall = false;
			isStart = false;
			isPath = false;
			weight = 0;
			hitbox.setFillColor(unvisitedColor);
		}

	}

	void deleteAsEnd() {
		if (isEnd) {
			isEnd = false;
			hitbox.setFillColor(unvisitedColor);
		}

	}


	void buildWall() {

		if (isStart || isEnd) {
			return;
		}

		if (!isWall) {
			isWall = true;
			hitbox.setFillColor(wallColor);
		}

	}

	void destroyWall() {

		if (isStart || isEnd) {
			return;
		}

		if (isWall) {
			isWall = false;
			hitbox.setFillColor(unvisitedColor);
		}

	}

	void setWeight(int weight) {
		this->weight = weight;
	}

	void setVisited(bool value) {
		
		if (value == true && visited == false) {
			visited = true;
			hitbox.setFillColor(visitedColor);
		}
		else if(value == false && visited == true){
			visited = false;
			hitbox.setFillColor(unvisitedColor);
		}
	}

	void setPath() {
		if (!isPath) {
			isPath = true;
			hitbox.setFillColor(pathColor);
		}
	}

	void deletePath() {
		if (isPath) {
			isPath = false;
			hitbox.setFillColor(visitedColor);
		}
	}

	int getWeight() {
		return weight;
	}

	bool IsWall() {
		return isWall;
	}

	bool IsPath() {
		return isPath;
	}

	bool IsStart() {
		return isStart;
	}

	bool IsEnd() {
		return isEnd;
	}
	sf::Vector2f getPos() {
		return pos;
	}

	sf::Vector2f getSize() {
		return size;
	}

	bool isVisited() {
		return visited;
	}

	bool isSelected(sf::Vector2f mousePos) {

		bool selected = false;
		sf::Vector2f position = hitbox.getPosition();
		sf::Vector2f size = hitbox.getSize();

		if (mousePos.x > position.x && mousePos.x < position.x + size.x)
		{
			if (mousePos.y > position.y && mousePos.y < position.y + size.y)
			{
				selected = true;
			}
		}

		return selected;
	}

	void render(sf::RenderWindow* window) {
		window->draw(hitbox);
	}

};