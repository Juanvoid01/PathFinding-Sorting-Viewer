#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"

class Grid
{

public:

	

	Grid(sf::Vector2f position, float squareSize, int numRows, int numColumns) {
		this->position = position;
		this->numRows = numRows;
		this->numColumns = numColumns;
		this->SquareSize = int(squareSize);
		gridWidth = numColumns * squareSize;
		gridHeight = numRows * squareSize;
		

		nodes = new Node*[numRows];

		sf::Vector2f nodeSize = sf::Vector2f(squareSize, squareSize);
		sf::Vector2f nodePos = position;

		for (int i = 0; i < numRows; i++) {
			nodes[i] = new Node[numColumns];
			for (int j = 0; j < numColumns; j++) {
				nodes[i][j] = Node(nodePos, nodeSize);
				nodePos.x += nodeSize.x;
			}
			nodePos.x = position.x;
			nodePos.y += nodeSize.y;
		}
		sf::Vector2i startNodePosition = sf::Vector2i(numRows / 2, numColumns / 2 - 10);
		sf::Vector2i endNodePosition = sf::Vector2i(numRows / 2, numColumns / 2 + 10);

		setAsStart(startNodePosition);
		setAsEnd(endNodePosition);

		sf::Vector2f toolBarPosition = this->position;
		toolBarPosition.x += 370.f;
		toolBarPosition.y -= 65.f;
		toolBar = new ToolBar(toolBarPosition);

		
		obstructionsManager = new ObstructionsManager(this,numRows,numColumns);
	}

	~Grid() {
		for (int i = 0; i < numRows; i++) {
			delete[] nodes[i];
		}
		delete[] nodes;

		delete toolBar;

		delete obstructionsManager;
	}

	sf::Vector2i getPosSelected(sf::Vector2f mousePos) {
		//calculate the selected square inside the grid

		if (!posInsideGrid(mousePos)) {
			return sf::Vector2i(-1, -1);
		}
		else {
			int row, col;
			mousePos -= position; //position relative to grid
			row = int(mousePos.y) / SquareSize;
			col = int(mousePos.x) / SquareSize;
			return sf::Vector2i(row, col);
		}
	}

	sf::Vector2i getNearestPos(sf::Vector2f mousePos) {
		//calculate the square inside the grid nearest to pos outside the grid

		if (!posInsideGrid(mousePos)) {

			if (mousePos.x < position.x) {
				mousePos.x = position.x;
			}
			else if (mousePos.x > position.x + gridWidth) {
				mousePos.x = position.x + gridWidth - 1.f;
			}

			if (mousePos.y < position.y) {
				mousePos.y = position.y;
			}
			else if (mousePos.y > position.y + gridHeight) {
				mousePos.y = position.y + gridHeight - 1.f;
			}
			 
		}

		return getPosSelected(mousePos);
	}

	void changePath(sf::Vector2i pos) {
		if (!validCoords(pos.x, pos.y)) {
			return;
		}

		if (isPath(pos.x, pos.y)) {
			nodes[pos.x][pos.y].deletePath();
		}
		else
		{
			nodes[pos.x][pos.y].setPath();
		}
	}

	void changeWall(sf::Vector2i pos) {

		if (!validCoords(pos.x,pos.y)) {
			return;
		}

		if (isWall(pos.x, pos.y)) {
			nodes[pos.x][pos.y].destroyWall();
		}
		else
		{
			nodes[pos.x][pos.y].buildWall();
		}
	}

	void setWall(sf::Vector2i pos, bool value) {
		if (!validCoords(pos.x, pos.y)) {
			return;
		}

		if (value == false) {
			nodes[pos.x][pos.y].destroyWall();
		}
		else
		{
			nodes[pos.x][pos.y].buildWall();
		}
	}
	
	void changeVisited(sf::Vector2i pos) {

		if (!validCoords(pos.x, pos.y)) {
			return;
		}

		if (isVisited(pos.x, pos.y)) {
			nodes[pos.x][pos.y].setVisited(false);
		}
		else
		{
			nodes[pos.x][pos.y].setVisited(true);
		}
	}

	void setEnableWeights(bool value) {
		if (value)
		{
			toolBar->enableWeights();
		}
		else
		{
			deleteObstructions();
			toolBar->disableWeights();
		}
	}

	void update(bool mouseClicked, bool mouseReleased, sf::Vector2f mousePos, bool& changeMade) {

		changeMade = false;
		if (state == GridState::Waiting) {

			if (mouseClicked) {
				toolBar->update(mousePos);
			}

			if (mouseClicked && isSelected(mousePos))
			{
				sf::Vector2i s = getPosSelected(mousePos);
				if (isStart(s.x, s.y)){
					state = GridState::MovingStart;
				}
				else if (isEnd(s.x, s.y)) {
					state = GridState::MovingEnd;
				}
				else
				{
					state = GridState::Walling;
				}
			}
		}
		else if (state == GridState::Walling)
		{
			if (mouseReleased)
			{
				state = GridState::Waiting;
			}

			sf::Vector2i posSelected = getPosSelected(mousePos);

			if (!isValidPosition(posSelected) || isStart(posSelected.x, posSelected.y) || isEnd(posSelected.x, posSelected.y))
			{
				return;
			}

			changeMade = true;

			if (toolBar->toolSelected == ToolBar::Tool::Waller) {
				setWall(posSelected, true);
				if (nodes[posSelected.x][posSelected.y].getWeight() > 0) {
					obstructionsManager->deleteObstruction(posSelected.x, posSelected.y);
					nodes[posSelected.x][posSelected.y].setWeight(0);
				}
			}
			else if (toolBar->toolSelected == ToolBar::Tool::Eraser) {
				setWall(posSelected, false);
				if (nodes[posSelected.x][posSelected.y].getWeight() != 0) {
					obstructionsManager->deleteObstruction(posSelected.x, posSelected.y);
					nodes[posSelected.x][posSelected.y].setWeight(0);
				}
				
			}
			else if (toolBar->toolSelected == ToolBar::Tool::SmallObstruction) {

				if (nodes[posSelected.x][posSelected.y].getWeight() != 5) {
					setWall(posSelected, false);
					obstructionsManager->setSmallObstruction(posSelected.x, posSelected.y, true);
					nodes[posSelected.x][posSelected.y].setWeight(5);
				}
				
			}
			else if (toolBar->toolSelected == ToolBar::Tool::BigObstruction) {
	
				if (nodes[posSelected.x][posSelected.y].getWeight() != 10) {
					setWall(posSelected, false);
					obstructionsManager->setBigObstruction(posSelected.x, posSelected.y, true);
					nodes[posSelected.x][posSelected.y].setWeight(10);
				}
				
			}
			

		}
		else if (state == GridState::MovingStart)
		{
			startNodeMarc.setCoords(mousePos);

			if (mouseReleased) {
				sf::Vector2i posSelected = getPosSelected(mousePos);

				if (posSelected != sf::Vector2i(-1, -1)) {
					//mouse outside grid
				
					setAsStart(posSelected);

				}
				else
				{
					//mouse inside grid
					setAsStart(getNearestPos(mousePos));
				}
				changeMade = true;
				state = GridState::Waiting;
			}
		}
		else if (state == GridState::MovingEnd)
		{
			endNodeMarc.setCoords(mousePos);

			if (mouseReleased) {
				sf::Vector2i posSelected = getPosSelected(mousePos);

				if (posSelected != sf::Vector2i(-1, -1)) {
					//mouse outside grid
					setAsEnd(posSelected);

				}
				else
				{
					//mouse inside grid
					setAsEnd(getNearestPos(mousePos));
				}
				changeMade = true;
				state = GridState::Waiting;
			}
		}
	}

	void render(sf::RenderWindow* window) {
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j< numColumns; j++) {
				nodes[i][j].render(window);
			}
		}
		startNodeMarc.render(window);
		endNodeMarc.render(window);
		toolBar->render(window);
		obstructionsManager->renderObstructions(window);
	}

	bool isSelected(sf::Vector2f mousePos) {
		return posInsideGrid(mousePos);
	}

	bool isVisited(int row, int col) {
		if (!validCoords(row, col)) {
			return false;
		}

		return nodes[row][col].isVisited();
	}

	bool isWall(int row, int col) {
		if (!validCoords(row, col)) {
			return false;
		}
		return nodes[row][col].IsWall();
	}

	bool isPath(int row, int col) {
		if (!validCoords(row, col)) {
			return false;
		}
		return nodes[row][col].IsPath();
	}

	bool isStart(int row, int col) {
		if (!validCoords(row, col)) {
			return false;
		}
		return nodes[row][col].IsStart();
	}
	bool isEnd(int row, int col) {
		if (!validCoords(row, col)) {
			return false;
		}
		return nodes[row][col].IsEnd();
	}
	int getWeight(int row, int col) {
		if (!validCoords(row, col)) {
			return -1;
		}
		return nodes[row][col].getWeight();
	}

	int getNumRows() {
		return numRows;
	}

	int getNumCols() {
		return numColumns;
	}

	sf::Vector2i getStartNode() {
		return startNodeMarc.getPosition();
	}

	sf::Vector2i getEndNode() {
		return endNodeMarc.getPosition();
	}

	bool isValidPosition(sf::Vector2i pos) {
		return pos.x >= 0 && pos.x < numRows&& pos.y >= 0 && pos.y < numColumns;
	}

	void deleteObstructions() {
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numColumns; j++)
			{
				if (nodes[i][j].getWeight() > 0)
				{
					nodes[i][j].setWeight(0);
					obstructionsManager->deleteObstruction(i, j);
				}
			}
		}
	}

	void deleteWalls() {
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numColumns; j++)
			{
				if (nodes[i][j].IsWall())
				{
					nodes[i][j].destroyWall();
				}
			}
		}
	}

	void resetVisitedNodes() {
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numColumns; j++)
			{

				if (nodes[i][j].IsPath())
				{
					nodes[i][j].deletePath();
				}
				if (nodes[i][j].isVisited())
				{
					nodes[i][j].setVisited(false);
				}
			}
		}
	}

	void resetNodes(){
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numColumns; j++)
			{
				if (nodes[i][j].IsWall())
				{
					nodes[i][j].destroyWall();
				}


				if (nodes[i][j].getWeight() > 0)
				{
					nodes[i][j].setWeight(0);
					obstructionsManager->deleteObstruction(i, j);
				}

				if (nodes[i][j].IsPath())
				{
					nodes[i][j].deletePath();
				}

				if (nodes[i][j].isVisited())
				{
					nodes[i][j].setVisited(false);
				}
			}
		}
	}

	void fillWithWalls() {
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numColumns; j++)
			{
				nodes[i][j].buildWall();
			}
		}
	}

private:

	sf::Vector2f position; //top-left corner

	Node** nodes;
	int numRows;
	int numColumns;
	int SquareSize;
	float gridWidth;
	float gridHeight;

	bool validCoords(int row, int col) {
		return row >= 0 && row < numRows&& col >= 0 && col < numColumns;
	}

	bool posInsideGrid(sf::Vector2f p) {
		sf::Vector2f endPos = sf::Vector2f(position.x + gridWidth, position.y + gridHeight);
		return p.x >= position.x && p.x <= endPos.x && p.y >= position.y && p.y <= endPos.y;
	}



	void setAsStart(sf::Vector2i pos) {

		sf::Vector2i laststart = startNodeMarc.getPosition();
		if (pos != endNodeMarc.getPosition()) {
			
			nodes[laststart.x][laststart.y].deleteAsStart();
			startNodeMarc = StartNodeMarc(nodes[pos.x][pos.y].getPos(), pos, SquareSize);
			nodes[pos.x][pos.y].setAsStart();
		}
		else {
			startNodeMarc = StartNodeMarc(nodes[laststart.x][laststart.y].getPos(), laststart, SquareSize);
			nodes[laststart.x][laststart.y].setAsStart();
		}

		
	}

	void setAsEnd(sf::Vector2i pos) {

		sf::Vector2i lastEnd = endNodeMarc.getPosition();

		if (pos != startNodeMarc.getPosition()) {
			nodes[lastEnd.x][lastEnd.y].deleteAsEnd();
			endNodeMarc = EndNodeMarc(nodes[pos.x][pos.y].getPos(), pos, SquareSize);
			nodes[pos.x][pos.y].setAsEnd();
		}
		else {
			endNodeMarc = EndNodeMarc(nodes[lastEnd.x][lastEnd.y].getPos(), lastEnd, SquareSize);
			nodes[lastEnd.x][lastEnd.y].setAsEnd();
		}
		
		
	}

	Node getNode(int row, int col) {
		return nodes[row][col];
	}

	struct EndNodeMarc {

		private :
		sf::CircleShape greatCircle;
		sf::CircleShape smallCircle;
		sf::Vector2i position;
		sf::Vector2f coords;
		public:

		sf::Vector2i getPosition() {
			return position;
		}
		sf::Vector2f getCoords() {
			return coords;
		}

		void setPosition(sf::Vector2i newPos) {
			position = newPos;
		}

		void setCoords(sf::Vector2f newCoords) {
			greatCircle.move(newCoords - coords);
			smallCircle.move(newCoords - coords);
			coords = newCoords;
			
		}

		EndNodeMarc() {
			position = sf::Vector2i(0, 0);
			coords = sf::Vector2f(0.0f, 0.0f);
		}
		EndNodeMarc(sf::Vector2f coords,sf::Vector2i position, int size) {

			this->position = position;
			this->coords = coords;
		
			sf::Vector2f center = coords + sf::Vector2f(float(size/2 - 1),float( size/2 - 1));
			greatCircle.setOutlineColor(sf::Color::Black);
			greatCircle.setOutlineThickness(size /9.f);
			greatCircle.setRadius(size / 3.f);
			greatCircle.setPosition(center - sf::Vector2f(greatCircle.getRadius(), greatCircle.getRadius()));
	

			smallCircle.setOutlineColor(sf::Color::Black);
			smallCircle.setOutlineThickness(size /12.f);
			smallCircle.setRadius(size / 6.f);
			smallCircle.setPosition(center - sf::Vector2f(float(smallCircle.getRadius()), float(smallCircle.getRadius())));

		}

		void render(sf::RenderWindow* window) {
			window->draw(greatCircle);
			window->draw(smallCircle);
		}
	};

	struct StartNodeMarc {

	private:
		sf::ConvexShape arrow;
		sf::Vector2i position;
		sf::Vector2f coords;
	public:
		sf::Vector2i getPosition() {
			return position;
		}
		sf::Vector2f getCoords() {
			return coords;
		}

		void setPosition(sf::Vector2i newPos) {
			position = newPos;
		}

		void setCoords(sf::Vector2f newCoords) {
			arrow.move(newCoords - coords);
			coords = newCoords;
		}

		StartNodeMarc() {
			position = sf::Vector2i(0, 0);
			coords = sf::Vector2f(0.0f, 0.0f);
		}

		StartNodeMarc(sf::Vector2f coords, sf::Vector2i position, int size) {

			this->position = position;
			this->coords = coords;
			
			arrow.setPointCount(6);

			arrow.setPoint(0, sf::Vector2f(0.f, 0.f));
			arrow.setPoint(1, sf::Vector2f(float(size) , float(size / 2)));
			arrow.setPoint(2, sf::Vector2f(0.f, float(size)));
			arrow.setPoint(3, sf::Vector2f(0.f, float(size - size/8)));
			arrow.setPoint(4, sf::Vector2f(float(size -size / 8), float(size / 2)));
			arrow.setPoint(5, sf::Vector2f(0.f, float(size / 8)));

			arrow.setFillColor(sf::Color::Black); 

			arrow.setPosition(coords);

		}

		void render(sf::RenderWindow* window) {
			window->draw(arrow);
		}
	};

	EndNodeMarc endNodeMarc;
	StartNodeMarc startNodeMarc;

	enum class GridState { Waiting, Walling, MovingStart, MovingEnd };
	GridState state = GridState::Waiting;

	struct RedCrossDrawing {

	private:

		sf::ConvexShape diagonalCross;
		sf::ConvexShape antiDiagonalCross;

		RedCrossDrawing() {

		}
	public:

		RedCrossDrawing(sf::Vector2f pos, float size) {

			sf::Vector2f endPoint(pos.x + size, pos.y + size);

			float width = size / 4.f;

			diagonalCross.setPointCount(4);
			
			diagonalCross.setPoint(0, pos);
			diagonalCross.setPoint(1, endPoint);
			diagonalCross.setPoint(2, sf::Vector2f(endPoint.x + width, endPoint.y));
			diagonalCross.setPoint(3, sf::Vector2f(pos.x + width, pos.y));

			diagonalCross.setFillColor(sf::Color::Red);

			antiDiagonalCross.setPointCount(4);

			antiDiagonalCross.setPoint(0, sf::Vector2f(endPoint.x, pos.y));
			antiDiagonalCross.setPoint(1, sf::Vector2f(pos.x, endPoint.y));
			antiDiagonalCross.setPoint(2, sf::Vector2f(pos.x + width, endPoint.y));
			antiDiagonalCross.setPoint(3, sf::Vector2f(endPoint.x + width, pos.y));

			antiDiagonalCross.setFillColor(sf::Color::Red);

		}
		
		void render(sf::RenderWindow* window) {
			window->draw(diagonalCross);
			window->draw(antiDiagonalCross);
		};
		
	};

	virtual class ObstructionDrawing {

	protected:
		RedCrossDrawing* redCross = nullptr;
	public:
		bool enabled = true;
		virtual void render(sf::RenderWindow* window) {

		};

		virtual char getType() {
			return 'o';
		};

		virtual ~ObstructionDrawing() {
			if (redCross!=nullptr)
			{
				delete redCross;
			}

		}
	};

	class SmallObstructionDrawing:public ObstructionDrawing {
	public:

		SmallObstructionDrawing(sf::Vector2f pos, int size) {

			this->position = position;
			this->size = size;

			river.append(sf::Vertex(sf::Vector2f(pos.x + size/4, pos.y), sf::Color::Blue));
			river.append(sf::Vertex(sf::Vector2f(pos.x + size, pos.y + size/4), sf::Color::Blue));
			river.append(sf::Vertex(sf::Vector2f(pos.x + size/8, pos.y + size / 2), sf::Color::Blue));
			river.append(sf::Vertex(sf::Vector2f(pos.x + size, pos.y + size/1.5), sf::Color::Blue));
			river.append(sf::Vertex(sf::Vector2f(pos.x, pos.y + size), sf::Color::Blue));

			redCross = new RedCrossDrawing(pos, size);
			
		}

		sf::Vector2f getPosition() {
			return position;
		}
		int getSize() {
			return size;
		}

		void render(sf::RenderWindow* window){
			window->draw(river);
			if (!enabled)
			{
				redCross->render(window);
			}
		}
		char getType(){
			return 's';
		}
	private:
		sf::VertexArray river = sf::VertexArray(sf::LinesStrip);
		int size;
		sf::Vector2f position;
		
	};

	class BigObstructionDrawing: public ObstructionDrawing {
	public:
		BigObstructionDrawing(sf::Vector2f pos,int size) {
			this->size = size;
			position = pos;

			mountain.setPointCount(3);

			mountain.setPoint(0, sf::Vector2f(float(size / 2), 0.f));
			mountain.setPoint(1, sf::Vector2f(0.f, float(size)));
			mountain.setPoint(2, sf::Vector2f(float(size), float(size)));

			mountain.setFillColor(sf::Color(126,64,0));

			mountain.setPosition(pos);

			redCross = new RedCrossDrawing(pos, size);
		}

		void render(sf::RenderWindow* window) {
			window->draw(mountain);
			if (!enabled)
			{
				redCross->render(window);
			}
		}
	private:
		int size;
		sf::Vector2f position;
		sf::ConvexShape mountain;

		char getType() {
			return 'b';
		}
	};

	struct NoneDrawing {
	private:
		sf::ConvexShape mouseIcon;

		NoneDrawing() {

		}
	public:

		NoneDrawing(sf::Vector2f pos, float size) {

			mouseIcon.setPointCount(7);


			mouseIcon.setPoint(0, sf::Vector2f(0.f, 0.f));
			mouseIcon.setPoint(1, sf::Vector2f(0.f, 5.f));
			mouseIcon.setPoint(2, sf::Vector2f(1.f, 4.f));
			mouseIcon.setPoint(3, sf::Vector2f(3.f, 6.f));
			mouseIcon.setPoint(4, sf::Vector2f(4.f, 6.f));
			mouseIcon.setPoint(5, sf::Vector2f(2.f, 4.f));
			mouseIcon.setPoint(6, sf::Vector2f(4.5f, 3.8f));

			mouseIcon.setPosition(pos);

			float lastSize = 6.f;
			float ratio = size / lastSize;
			mouseIcon.setScale(ratio, ratio);
			mouseIcon.setFillColor(sf::Color::White);
			mouseIcon.setOutlineColor(sf::Color(150, 150, 150));
		}

		void render(sf::RenderWindow* window) {
			window->draw(mouseIcon);
		}

	};

	class ToolBar {
	public:
		enum class Tool{Eraser,Waller,SmallObstruction,BigObstruction, None};

		Tool toolSelected;
		sf::Vector2f position;
		bool weightsEnabled = true;

		ToolBar(sf::Vector2f pos) {
			position = pos;
			toolSelected = Tool::Waller;

			sf::Vector2f posi = pos;
			sf::Vector2f size = sf::Vector2f(25.f, 25.f);

			eraser.setPosition(posi);
			eraser.setSize(size);
			eraser.setFillColor(sf::Color(150, 150, 150));
			eraser.setOutlineColor(borderColor);
			eraser.setOutlineThickness(3.f);

			posi.x += 50.f;
			waller.setPosition(posi);
			waller.setSize(size);
			waller.setFillColor(sf::Color(256, 256, 256));
			waller.setOutlineColor(selectedColor);
			waller.setOutlineThickness(3.f);

			posi.x += 50.f;
			smallObstruction.setPosition(posi);
			smallObstruction.setSize(size);
			smallObstruction.setFillColor(sf::Color(256, 256, 256));
			smallObstruction.setOutlineColor(borderColor);
			smallObstruction.setOutlineThickness(3.f);
			smallObstructionDrawing = new SmallObstructionDrawing(sf::Vector2f(posi.x + 2.f, posi.y + 2.f), size.x - 4.f);

			posi.x += 50.f;
			bigObstruction.setPosition(posi);
			bigObstruction.setSize(size);
			bigObstruction.setFillColor(sf::Color(256, 256, 256));
			bigObstruction.setOutlineColor(borderColor);
			bigObstruction.setOutlineThickness(3.f);
			bigObstructionDrawing = new BigObstructionDrawing(sf::Vector2f(posi.x + 2.f,posi.y + 2.f), size.x - 4.f);

			posi.x += 50.f;
			none.setPosition(posi);
			none.setSize(size);
			none.setFillColor(sf::Color(256, 256, 256));
			none.setOutlineColor(borderColor);
			none.setOutlineThickness(3.f);

			posi.x += 5.f;
			posi.y += 1.f;
			noneDrawing = new NoneDrawing(posi, size.x - 2.f);
		}

		~ToolBar() {
			delete smallObstructionDrawing;
			delete bigObstructionDrawing;
			delete noneDrawing;
		}

		void enableWeights() {
			smallObstructionDrawing->enabled = true;
			bigObstructionDrawing->enabled = true;
			weightsEnabled = true;
		}

		void disableWeights() {
			smallObstructionDrawing->enabled = false;
			bigObstructionDrawing->enabled = false;
			weightsEnabled = false;
			if (toolSelected == Tool::SmallObstruction || toolSelected == Tool::BigObstruction)
			{
				changeSelected(Tool::None);
			}
		}

		void update(sf::Vector2f mousePos) {

			if (eraser.getGlobalBounds().contains(mousePos)) {
				if (toolSelected!=Tool::Eraser) {
					changeSelected(Tool::Eraser);
				}
			}
			else if (waller.getGlobalBounds().contains(mousePos)) {
				if (toolSelected != Tool::Waller) {
					changeSelected(Tool::Waller);
				}
			}
			else if (weightsEnabled && smallObstruction.getGlobalBounds().contains(mousePos)) {
				if (toolSelected != Tool::SmallObstruction) {
					changeSelected(Tool::SmallObstruction);
				}
			}
			else if (weightsEnabled && bigObstruction.getGlobalBounds().contains(mousePos)) {
				if (toolSelected != Tool::BigObstruction) {
					changeSelected(Tool::BigObstruction);
				}
			}
			else if (none.getGlobalBounds().contains(mousePos)) {
				if (toolSelected != Tool::None) {
					changeSelected(Tool::None);
				}
			}

		}

		void render(sf::RenderWindow* window) {
			window->draw(eraser);
			window->draw(waller);
			window->draw(smallObstruction);
			window->draw(bigObstruction);
			window->draw(none);

			smallObstructionDrawing->render(window);
			bigObstructionDrawing->render(window);
			noneDrawing->render(window);
		}
	private:

		sf::RectangleShape eraser;
		sf::RectangleShape waller;
		sf::RectangleShape smallObstruction;
		sf::RectangleShape bigObstruction;


		sf::RectangleShape none;

		sf::Color selectedColor = sf::Color(200, 50, 50);
		sf::Color borderColor = sf::Color(200, 200, 200);

		SmallObstructionDrawing* smallObstructionDrawing;
		BigObstructionDrawing* bigObstructionDrawing;
		NoneDrawing* noneDrawing;

		void changeSelected(Tool newTool) {

			if (toolSelected == Tool::Eraser) {
				eraser.setOutlineColor(borderColor);
			}
			else if (toolSelected == Tool::Waller) {
				waller.setOutlineColor(borderColor);
			}
			else if (toolSelected == Tool::SmallObstruction) {
				smallObstruction.setOutlineColor(borderColor);
			}
			else if (toolSelected == Tool::BigObstruction) {
				bigObstruction.setOutlineColor(borderColor);
			}
			else if (toolSelected == Tool::None) {
				none.setOutlineColor(borderColor);
			}


			if (newTool == Tool::Eraser) {
				toolSelected = Tool::Eraser;
				eraser.setOutlineColor(selectedColor);
			}
			else if (newTool == Tool::Waller) {
				toolSelected = Tool::Waller;
				waller.setOutlineColor(selectedColor);
			}
			else if (newTool == Tool::SmallObstruction) {
				toolSelected = Tool::SmallObstruction;
				smallObstruction.setOutlineColor(selectedColor);
			}
			else if (newTool == Tool::BigObstruction) {
				toolSelected = Tool::BigObstruction;
				bigObstruction.setOutlineColor(selectedColor);
			}
			else if (newTool == Tool::None) {
				toolSelected = Tool::None;
				none.setOutlineColor(selectedColor);
			}
		}
	};


	ToolBar* toolBar;


	class ObstructionsManager {
	public:
		ObstructionsManager(Grid* parent, int rows,int cols) {
			numRows = rows;
			numCols = cols;
			this->parent = parent;

			obstructions = new ObstructionDrawing**[rows];
			for (int row = 0; row < rows; row++)
			{
				obstructions[row] = new ObstructionDrawing*[cols];
				for (int col = 0; col < cols; col++)
				{
					obstructions[row][col] = nullptr;
				}
			}

		}
		~ObstructionsManager() {
			for (int row = 0; row < numRows; row++)
			{
				for (int col = 0; col < numCols; col++)
				{
					if (obstructions[row][col]!= nullptr)
					{
						delete obstructions[row][col];
					}
				}
				delete[] obstructions[row];
			}
			delete[] obstructions;
		}

		void setSmallObstruction(int row, int col, bool value) {
			if (value == true)
			{
				if (obstructions[row][col]==nullptr)
				{
					numberOfObstructions++;
					obstructions[row][col] = new SmallObstructionDrawing(parent->getNode(row,col).getPos(), parent->SquareSize);
				}
				else if(obstructions[row][col]->getType() == 'b')
				{
					delete obstructions[row][col];
					obstructions[row][col] = new SmallObstructionDrawing(parent->getNode(row, col).getPos(), parent->SquareSize);
				}
			}
			else {
				if (obstructions[row][col] != nullptr && obstructions[row][col]->getType() == 's')
				{
					delete obstructions[row][col];
					obstructions[row][col] = nullptr;
					numberOfObstructions--;
				}
			}
		}

		void setBigObstruction(int row, int col, bool value) {
			if (value == true)
			{
				if (obstructions[row][col] == nullptr)
				{
					numberOfObstructions++;
					obstructions[row][col] = new BigObstructionDrawing(parent->getNode(row, col).getPos(), parent->SquareSize);
				}
				else if (obstructions[row][col]->getType() == 's')
				{
					delete obstructions[row][col];
					obstructions[row][col] = new BigObstructionDrawing(parent->getNode(row, col).getPos(), parent->SquareSize);
				}
			}
			else {
				if (obstructions[row][col] != nullptr && obstructions[row][col]->getType() == 'b')
				{
					delete obstructions[row][col];
					obstructions[row][col] = nullptr;
					numberOfObstructions--;
				}
			}
		}
		
		void deleteObstruction(int row, int col) {
			if (obstructions[row][col] != nullptr)
			{
				delete obstructions[row][col];
				obstructions[row][col] = nullptr;
				numberOfObstructions--;
			}
		}

		void renderObstructions(sf::RenderWindow* window) {

			if (numberOfObstructions == 0) return;

			int ObstructionsLeft = numberOfObstructions;

			for (int row = 0; row < numRows; row++)
			{
				for (int col = 0; col < numCols; col++)
				{
					if (obstructions[row][col] != nullptr)
					{
						obstructions[row][col]->render(window);
						ObstructionsLeft--;
						if (ObstructionsLeft == 0) return;
					}
				}
			}
		}
	private:
		Grid* parent;
		ObstructionDrawing*** obstructions;
		int numRows;
		int numCols;
		int numberOfObstructions = 0;
	};

	

	ObstructionsManager* obstructionsManager;
	
	

};