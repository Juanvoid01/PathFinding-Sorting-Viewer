#pragma once

#include "Object.h"


class Button : public Object{

private:
	sf::Sprite hitbox;

	bool selected;
public:

	Button(sf::Vector2f pos, std::string id, sf::Texture* texture)
		:Object(pos, sf::Vector2f(texture->getSize()),id)
	{
		
		hitbox.setPosition(pos);
		hitbox.setTexture(*texture);
		selected = false;
	}

	bool isSelected(sf::Vector2f mousePos) {

		bool selected = false;

		if (mousePos.x > position.x && mousePos.x < position.x + size.x)
		{
			if (mousePos.y > position.y && mousePos.y < position.y + size.y)
			{
				selected = true;
			}
		}

		return selected;
	}

	void setPosition(sf::Vector2f pos)
	{
		Object::setPosition(pos);
		this->hitbox.setPosition(pos);
	}
	void setColor(sf::Color color) {

	}

	void setScale(sf::Vector2f scale) {
		hitbox.setScale(scale);
		setSize(sf::Vector2f(getSize().x * scale.x, getSize().y * scale.y));
	}

	sf::Color getColor() {
		return sf::Color(0, 0, 0);
	}

	void update() {

	}

	void update(sf::Vector2f mousePos) {
		if (selected) {

			if (!isSelected(mousePos)) {
				selected = false;
				hitbox.setScale(hitbox.getScale() / 1.1f);
			}
		}
		else
		{
			if (isSelected(mousePos)) {
				selected = true;
				hitbox.setScale(hitbox.getScale()*1.1f);
			}
		}
	}

	void render(sf::RenderWindow* window)
	{
		window->draw(hitbox);
	}

};