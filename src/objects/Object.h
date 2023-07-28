#pragma once

#include <SFML/Graphics.hpp>
#include <string>

const sf::Vector2f CEROVECTOR2F = sf::Vector2f(0.f, 0.f);
const sf::Vector2f CEROVECTOR2I = sf::Vector2f(0, 0);

class Object
{

protected:
	sf::Vector2f position;
	sf::Vector2f size;
	std::string id;

public:
	
	Object(sf::Vector2f pos, sf::Vector2f size, std::string id) {
		this->setPosition(pos);
		this->setSize(size);
	}

	sf::Vector2f getPosition() {
		return this->position;
	}
	sf::Vector2f getSize() {
		return this->size;
	}
	std::string getId() {
		return this->id;
	}

	void setPosition(sf::Vector2f pos) {
		this->position = pos;
	}

	void setSize(sf::Vector2f size) {
		this->size = size;
	}

	void setId(std::string id) {
		this->id = id;
	}

	virtual void update() = 0;

	virtual void setColor(sf::Color color) = 0;

	virtual sf::Color getColor() = 0;

	virtual void render(sf::RenderWindow* window) = 0;

	bool operator ==(const Object& other) {
		return this->id == other.id;
	}

};

