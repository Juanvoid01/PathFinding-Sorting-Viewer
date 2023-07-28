#pragma once
#include <SFML/Graphics.hpp>

class BallLineSelector{

	enum class State{Waiting, Selected};
private:

	int value;
	int minValue;
	int maxValue;
	sf::RectangleShape line;
	sf::CircleShape ball;

	sf::Vector2f minPos;
	sf::Vector2f maxPos;

	State state;

	void moveBall(float newPos) {

		int range = maxValue - minValue;

		float newOfset = newPos - minPos.x;
		float maxOfset = maxPos.x - minPos.x;

		float newVal = range * newOfset / maxOfset;

		if (newVal <= minValue)
		{
			ball.setPosition(minPos);
			value = minValue;
		}
		else if (newVal >= maxValue)
		{
			ball.setPosition(maxPos);
			value = maxValue;
		}
		else
		{
			ball.setPosition(sf::Vector2f(newPos - ball.getRadius(), ball.getPosition().y));
			value = newVal;
		}

		

	}

public:

	BallLineSelector(sf::Vector2f pos, sf::Vector2f size, int minValue = 0, int maxValue = 100) {

		state = State::Waiting;
		this->minValue = minValue;
		this->maxValue = maxValue;
		value = minValue;
		line.setPosition(pos);
		line.setSize(size);
		ball.setRadius(1.5f * size.y);

		minPos = sf::Vector2f(pos.x - ball.getRadius(), pos.y - ball.getRadius() + size.y / 2.f);
		maxPos = sf::Vector2f(pos.x + size.x - ball.getRadius(), pos.y - ball.getRadius() + size.y / 2.f);

		ball.setPosition(minPos);
		ball.setFillColor(sf::Color(150,150,150));
		line.setFillColor(sf::Color(50, 50, 50));

	}

	void setValue(int newValue) {
		if (newValue <= minValue)
		{
			moveBall(minPos.x);
			this->value = minValue;
		}
		else if (newValue >= maxValue)
		{
			moveBall(maxPos.x);
			this->value = maxValue;
		}
		else
		{
			float newOfset = newValue * (maxPos.x - minPos.x) / maxValue;
			moveBall(minPos.x + newOfset + ball.getRadius());
			this->value = newValue;
		}
	}

	void update(bool MouseClick,bool MouseRelease, sf::Vector2f mousePos) {

		if (state == State::Waiting)
		{
			if (MouseClick && line.getGlobalBounds().contains(mousePos))
			{
				state = State::Selected;
			}
		}
		else if (state == State::Selected)
		{
			moveBall(mousePos.x);

			if (MouseRelease)
			{
				state = State::Waiting;
			}
		}
	}

	void render(sf::RenderWindow* window)
	{

		window->draw(line);
		window->draw(ball);
	}

	int getValue() {
		return value;
	}

	void move(sf::Vector2f movement) {
		line.move(movement);
		ball.move(movement);
	}

	sf::Vector2f getPosition() {
		return line.getPosition();
	}

	sf::Vector2f getSize() {
		return line.getSize();
	}

};