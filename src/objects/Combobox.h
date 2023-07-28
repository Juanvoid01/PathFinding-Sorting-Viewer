#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class Combobox {



public:

	Combobox(sf::Vector2f position, sf::Font* font, float boxSize = 20.f) {

		this->boxSize = boxSize;
		this->font = font;
		hitbox.setPosition(position);
		hitbox.setSize(sf::Vector2f(5.f, 5.f));
		hitbox.setOutlineColor(sf::Color::Black);
		hitbox.setOutlineThickness(1.0f);
		hitbox.setFillColor(sf::Color(255, 255, 255, 100));



		square.setOutlineColor(sf::Color::Black);
		square.setOutlineThickness(1.0f);
		square.setPosition(position.x, position.y);
		square.setSize(sf::Vector2f(boxSize, boxSize));
		square.setFillColor(sf::Color(150, 150, 150));

		triangle.setPointCount(3);
		triangle.setFillColor(sf::Color::Black);
		triangle.setPoint(0, sf::Vector2f(0.0, 0.0));
		triangle.setPoint(1, sf::Vector2f(2.0, 0.0));
		triangle.setPoint(2, sf::Vector2f(1.0, 1.0));
		triangle.setPosition(position);
		triangle.setScale(20.f/3.f, 20.f / 3.f);
	}

	void addElement(std::string text) {
		if (selectedItem == -1)
		{
			hitbox.setSize(sf::Vector2f(0.f, 0.f));
			selectedItem = 0;
		}
		
		sf::Vector2f hitboxPos = hitbox.getPosition();
		sf::Text t(text, *font, boxSize - 5.f);
		t.setPosition(hitboxPos.x + 1.f, hitboxPos.y + boxSize * strings.size());
		t.setFillColor(sf::Color::Black);



		sf::RectangleShape r;

		r.setFillColor(normalBoxColor);

		r.setPosition(hitboxPos.x, hitboxPos.y + boxSize * strings.size());

		strings.emplace_back(t);

		float w = t.getGlobalBounds().width;


		if (w > hitbox.getSize().x)
		{
			hitbox.setSize(sf::Vector2f(w, hitbox.getSize().y));

			for (auto& it : boxes) {
				it.setSize(sf::Vector2f(w, it.getSize().y));
			}

			square.setPosition(hitbox.getPosition().x + w, hitbox.getPosition().y);
			triangle.setPosition(square.getPosition() + sf::Vector2f(4.f,6.f));

		}

		r.setSize(sf::Vector2f(hitbox.getSize().x, boxSize));

		if (boxes.empty())
		{

			r.setOutlineColor(sf::Color::Black);
			r.setOutlineThickness(1.0f);
		}

		boxes.emplace_back(r);

		hitbox.setSize(sf::Vector2f(hitbox.getSize().x, hitbox.getSize().y + boxSize));

	}

	std::string getValue() {
		if (strings.empty())
		{
			return "EMPTY";
		}
		return strings[selectedItem].getString();
	}

	void update(bool mouseClick, sf::Vector2f mousePos) {

		if (state == State::Retracted)
		{

			if (mouseClick && (boxes[0].getGlobalBounds().contains(mousePos) || square.getGlobalBounds().contains(mousePos)))
			{
				state = State::Extended;
			}
		}
		else if (state == State::Extended)
		{
			if (isSelected(mousePos))
			{
				int i = 0;
				for (auto& box : boxes) {
					if (box.getGlobalBounds().contains(mousePos))
					{
						box.setFillColor(selectedBoxColor);

						if (mouseClick && i != 0)
						{
							sf::Vector2f posAux = strings[0].getPosition();

							strings[0].setPosition(strings[i].getPosition());
							strings[i].setPosition(posAux);

							std::swap(strings[0], strings[i]);

							box.setFillColor(normalBoxColor);
							state = State::Retracted;
							break;
						}
					}
					else
					{
						box.setFillColor(normalBoxColor);
					}
					i++;
				}
			}
			else
			{
				if (mouseClick)
				{
					for (auto& box : boxes)
					{
						box.setFillColor(normalBoxColor);
					}

					state = State::Retracted;
				}
			}

		}

	}
	void render(sf::RenderWindow* window) {

		if (state == State::Retracted)
		{
			if (!boxes.empty())
			{
				window->draw(boxes[0]);
				window->draw(strings[0]);
			}
		}
		else if (state == State::Extended)
		{
			window->draw(hitbox);
			for (auto& it : boxes) {
				window->draw(it);
			}

			for (auto& it : strings) {
				window->draw(it);
			}
		}
		window->draw(square);
		window->draw(triangle);

	}
private:

	sf::RectangleShape hitbox;
	std::vector<sf::Text> strings;
	std::vector<sf::RectangleShape> boxes;
	unsigned int selectedItem = -1;
	float boxSize = 15.f;
	sf::Font* font;

	sf::Color selectedBoxColor = sf::Color(0, 0, 220, 100);
	sf::Color normalBoxColor = sf::Color(150, 150, 150, 100);

	enum class State { Retracted, Extended };

	State state = State::Retracted;

	sf::ConvexShape triangle;
	sf::RectangleShape square;

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
};
