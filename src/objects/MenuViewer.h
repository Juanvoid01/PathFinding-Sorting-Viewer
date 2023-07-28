#pragma once


#include "Button.h"
#include <random>

class MenuViewer
{
private:

	sf::Text* title;

	Button* exitButton;
	Button* sortingButton;
	Button* pathfindButton;
	

	struct Serpent {

	private:

		sf::Vector2f pos;
		char favouriteDir;

		sf::RectangleShape** parts;
		sf::RectangleShape zone;
		std::random_device random_device;

		sf::Clock timer;
		sf::Time elapsed;
		sf::Color normalColor = sf::Color(0, 100, 0);
		sf::Color selectedColor = sf::Color(100, 0, 0);

		char directions[4] = { '0','0','0','0' };
		float frequency;
		sf::Vector2f spawn;

		int snakeSize;
		Serpent() {

		}

	public:

		Serpent(sf::RectangleShape zone, int snakeSize, sf::Vector2f spawn, char favouriteDir = 'l', float frequency = 0.1f) {
			pos = spawn;

			this->zone = zone;
			this->snakeSize = snakeSize;
			this->favouriteDir = favouriteDir;
			this->frequency = frequency;
			this->spawn = spawn;
			parts = new sf::RectangleShape * [snakeSize];
			

			for (int i = 0; i < snakeSize; i++)
			{
				parts[i] = new sf::RectangleShape();
				parts[i]->setPosition(pos);
				parts[i]->setSize(sf::Vector2f(10.f,10.f));
				pos.y += 10.f;
				parts[i]->setFillColor(normalColor);
				parts[i]->setOutlineThickness(0.5f);
				parts[i]->setOutlineColor(sf::Color::Black);

			}
			pos = parts[0]->getPosition();

		}

		~Serpent() {
			for (int i = 0; i < snakeSize; i++)
			{
				delete parts[i];
			}
			delete[] parts;
		}
		
		bool posInsideSnake(sf::Vector2f pos) {
			bool result = false;
			int i = 0;
			while (!result && i < snakeSize)
			{
				result = parts[i]->getGlobalBounds().contains(pos);
				i++;
			}

			return result;
		}

		bool posInsideZone(sf::Vector2f pos) {

			bool inside = false;
			sf::Vector2f zonePos = zone.getPosition();
			sf::Vector2f size = zone.getSize();
			if (pos.x > zonePos.x && pos.x < zonePos.x + size.x)
			{
				if (pos.y > zonePos.y && pos.y < zonePos.y + size.y)
				{
					inside = true;
				}
			}

			return inside;
		}

		void update(bool mouseClicked, sf::Vector2f mousePos) {

			elapsed = timer.getElapsedTime();
			if (elapsed.asSeconds() > frequency)
			{
				timer.restart();
				int numOfDirections = 0;

				sf::Vector2f top(pos.x, pos.y - 10.f);
				sf::Vector2f bottom(pos.x, pos.y + 11.f);
				sf::Vector2f left(pos.x - 10.f, pos.y);
				sf::Vector2f right(pos.x + 11.f, pos.y);

				if (posInsideZone(top) && !posInsideSnake(top))
				{
					directions[numOfDirections++] = 't';
				}
				if (posInsideZone(bottom) && !posInsideSnake(bottom))
				{
					directions[numOfDirections++] = 'b';
				}
				if (posInsideZone(left) && !posInsideSnake(left))
				{
					directions[numOfDirections++] = 'l';
				}
				if (posInsideZone(right) && !posInsideSnake(right))
				{
					directions[numOfDirections++] = 'r';
				}

				if (numOfDirections > 0)
				{
					std::mt19937 random_generator(random_device());

					std::uniform_int_distribution<int> random_dir(0, numOfDirections);

					int rn = random_dir(random_generator);

					char dir = rn == numOfDirections ? favouriteDir : directions[rn];

					if (dir == 't')
					{
						pos = top;
					}
					else if (dir == 'b')
					{
						pos = bottom;
					}
					else if (dir == 'l')
					{
						pos = left;
					}
					else if (dir == 'r')
					{
						pos = right;
					}
					

					for (int i = snakeSize - 1; i > 0; i--)
					{
						parts[i]->setPosition(parts[i - 1]->getPosition());
					}
					parts[0]->setPosition(pos);
				}
				else
				{

					pos = spawn;

					for (int i = 0; i < snakeSize; i++)
					{
						parts[i]->setPosition(pos);
						pos.y += 10.f;
					}
					pos = parts[0]->getPosition();
					
				}

			}
			if (mouseClicked)
			{
				for (int i = 0; i < snakeSize; i++)
				{

					if (parts[i]->getGlobalBounds().contains(mousePos))
					{
						if (parts[i]->getFillColor() == normalColor)
						{
							parts[i]->setFillColor(selectedColor);
						}
						else
						{
							parts[i]->setFillColor(normalColor);
						}
					}
				}
			}



		}
		void render(sf::RenderWindow* window) {
			for (int i = 0; i < snakeSize; i++)
			{
				window->draw(*parts[i]);
			}
		}

	};

	Serpent* serpent1;
	Serpent* serpent2;

public:

	bool exitAction = false;
	bool sortingAction = false;
	bool pathFindAction = false;

	MenuViewer(std::unordered_map<std::string, sf::Texture*>& textures, const sf::Font& font) {


		title = new sf::Text("ALGORITHM VIEWER", font, 50);
		title->setPosition(260.0f, 10.0f);
		//title->setOutlineColor(sf::Color(200, 200, 255));
		title->setOutlineThickness(1);
		title->setFillColor(sf::Color::Black);


		sortingButton = new Button(sf::Vector2f(400, 200.f), "SortingButton", textures["SortingButton"]);
		pathfindButton = new Button(sf::Vector2f(400.f, 300.f), "PathfindButton", textures["PathfindButton"]);
		exitButton = new Button(sf::Vector2f(400.f, 400.f), "ExitButton", textures["ExitButton"]);

		sortingButton->setScale(sf::Vector2f(0.5f, 0.5f));
		pathfindButton->setScale(sf::Vector2f(0.5f, 0.5f));
		exitButton->setScale(sf::Vector2f(0.5f, 0.5f));
		sf::RectangleShape r;
		r.setPosition(0.f, 0.f);
		r.setSize(sf::Vector2f(1000.f, 600.f));
		serpent1 = new Serpent(r, 5, sf::Vector2f(250.f, 550.f), 't', 0.1f);
		serpent2 = new Serpent(r, 7, sf::Vector2f(750.f, 25.f), 'b', 0.12);
	}

	~MenuViewer() {
		delete exitButton;
		delete sortingButton;
		delete pathfindButton;
		delete title;
		delete serpent1;
		delete serpent2;

	}

	void update(bool mouseClicked, sf::Vector2f mousePos) {

		exitButton->update(mousePos);
		sortingButton->update(mousePos);
		pathfindButton->update(mousePos);
		serpent1->update(mouseClicked, mousePos);
		serpent2->update(mouseClicked, mousePos);
		if (mouseClicked)
		{
			if (exitButton->isSelected(mousePos))
			{
				exitAction = true;
			}
			else if (sortingButton->isSelected(mousePos))
			{
				sortingAction = true;
			}
			else if (pathfindButton->isSelected(mousePos))
			{
				pathFindAction = true;
			}
		}
	}

	void render(sf::RenderWindow* window) {

		serpent1->render(window);
		serpent2->render(window);

		window->draw(*title);

		exitButton->render(window);
		sortingButton->render(window);
		pathfindButton->render(window);

		

	}
};

