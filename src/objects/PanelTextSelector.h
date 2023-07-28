#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class PanelTextSelector {
	enum class State {Retracted, Extended};

private:
	std::string value;
	State state;
	std::vector<sf::Text> items;
	std::vector<sf::RectangleShape> boxes;
	sf::Vector2f position;
	sf::Vector2f itemSize;

	float maxWidth;

public:

	PanelTextSelector(sf::Vector2f pos, sf::Vector2f itemSize) {
		state = State::Retracted;
		position = pos;
		this->itemSize = itemSize;
		this->value = "";
		maxWidth = 0;
	}

	void addItem(std::string value, const sf::Font& font) {
		if (items.empty())
		{
			this->value = value;
		}

		sf::Text t = sf::Text(value, font, 15);

		t.setPosition(position.x, position.y + itemSize.y * items.size());
		t.setFillColor(sf::Color::Black);
		items.emplace_back(t);

		sf::RectangleShape r;
		r.setFillColor(sf::Color(250, 250, 250));
		r.setOutlineColor(sf::Color(30, 30, 30));
		r.setOutlineThickness(2);
		r.setPosition(t.getPosition());
		float w = t.getGlobalBounds().width;
		float h = t.getGlobalBounds().height;

		if (w > maxWidth)
		{
			maxWidth = w;
			for (auto& it : boxes) {
				it.setSize(sf::Vector2f(w,it.getSize().y));
			}
		}

		r.setSize(sf::Vector2f(maxWidth, h * 1.5f));

		boxes.emplace_back(r);
	}

	void update(bool mouseClick,sf::Vector2f mousePos) {

		if (state == State::Retracted)
		{

			if (mouseClick && boxes[0].getGlobalBounds().contains(mousePos))
			{
				state = State::Extended;
			}
		}
		else if (state == State::Extended)
		{
			if (mouseClick)
			{
				for (auto& it : items) {
					if (it.getGlobalBounds().contains(mousePos))
					{
						value = it.getString();
						sf::Vector2f aux = items[0].getPosition();
						items[0].setPosition(it.getPosition());
						it.setPosition(aux);


						std::swap(it,items[0]);
						
						state = State::Retracted;
						break;
					}
				}
				state = State::Retracted;
			}

		}
	}

	void render(sf::RenderWindow* window) {

		if (state == State::Retracted)
		{
			window->draw(boxes[0]);
			window->draw(items[0]);
		}
		else if (state == State::Extended)
		{
			for (auto& it : boxes) {
				window->draw(it);
			}

			for (auto& it : items) {
				window->draw(it);
			}
		}

	}

	std::string getValue() {
		return value;
	}


};