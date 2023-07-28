#pragma once

#include "Object.h"

class Rectangle: public Object
{
private:

	sf::RectangleShape hitbox;
public:

	Rectangle(sf::Vector2f pos = CEROVECTOR2F, sf::Vector2f size = CEROVECTOR2F, std::string id = "")
		:Object(pos, size, id)
	{
		this->setPosition(pos);
		this->setSize(size);

		this->setColor(sf::Color(0,200,0));

	}

	void setPosition(sf::Vector2f pos)
	{
		Object::setPosition(pos);
		this->hitbox.setPosition(pos);
	}

	void setSize(sf::Vector2f size) {
		Object::setSize(size);
		this->hitbox.setSize(size);
	}

	void setColor(sf::Color color) {
		this->hitbox.setFillColor(color);
	}

	sf::Color getColor() {
		return this->hitbox.getFillColor();
	}

	void update() {

	}

	void render(sf::RenderWindow* window)
	{
		window->draw(hitbox);
	}
};

