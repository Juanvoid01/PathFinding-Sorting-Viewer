#include "GUI.h"



void GUI::initVariables()
{
	this->window = nullptr;
	this->simulation = nullptr;
	this->state = ProgramState::in_menu;
	this->menuViewer = nullptr;
	this->sortingViewer = nullptr;
	this->defaultFont.loadFromFile("data/fonts/arial.ttf");

}

void GUI::initWindow()
{
	this->videoMode.width = this->W_WIDTH;
	this->videoMode.height = this->W_HEIGHT;
	this->window = new sf::RenderWindow(this->videoMode, "Algorithm Viewer", sf::Style::Resize | sf::Style::Close);
	this->window->setFramerateLimit(this->fps);
	this->lastWindowSize = this->window->getSize();
}

void GUI::initTextures() {

	textures["ExitButton"] = new sf::Texture();
	textures["PlayButton"] = new sf::Texture();
	textures["PathfindButton"] = new sf::Texture();
	textures["SortingButton"] = new sf::Texture();
	textures["ForwardButton"] = new sf::Texture();
	textures["BackwardButton"] = new sf::Texture();
	textures["PlayForwardButton"] = new sf::Texture();
	textures["PlayBackwardButton"] = new sf::Texture();
	textures["ResetButton"] = new sf::Texture();
	textures["PauseButton"] = new sf::Texture();
	textures["BackButton"] = new sf::Texture();

	textures["ExitButton"]->loadFromFile("data/designs/ExitButton.png");
	textures["PlayButton"]->loadFromFile("data/designs/PlayButton.png");
	textures["PathfindButton"]->loadFromFile("data/designs/PathfindButton.png");
	textures["SortingButton"]->loadFromFile("data/designs/SortingButton.png");
	textures["ForwardButton"]->loadFromFile("data/designs/ForwardButton.png");
	textures["BackwardButton"]->loadFromFile("data/designs/BackwardButton.png");
	textures["PlayForwardButton"]->loadFromFile("data/designs/PlayForwardButton.png");
	textures["PlayBackwardButton"]->loadFromFile("data/designs/PlayBackwardButton.png");
	textures["ResetButton"]->loadFromFile("data/designs/ResetButton.png");
	textures["PauseButton"]->loadFromFile("data/designs/PauseButton.png");
	textures["BackButton"]->loadFromFile("data/designs/BackButton.png");
}

void GUI::deleteTextures() {
	delete textures["ExitButton"];
	textures.erase("ExitButton");

	delete textures["PlayButton"];
	textures.erase("PlayButton");

	delete textures["PathfindButton"];
	textures.erase("PathfindButton");

	delete textures["SortingButton"];
	textures.erase("SortingButton");

	delete textures["ForwardButton"];
	textures.erase("ForwardButton");

	delete textures["BackwardButton"];
	textures.erase("BackwardButton");

	delete textures["PlayForwardButton"];
	textures.erase("PlayForwardButton");

	delete textures["PlayBackwardButton"];
	textures.erase("PlayBackwardButton");

	delete textures["ResetButton"];
	textures.erase("ResetButton");

	delete textures["PauseButton"];
	textures.erase("PauseButton");

	delete textures["BackButton"];
	textures.erase("BackButton");

}



GUI::GUI()
{
	this->initTextures();
	this->initVariables();
	this->initWindow();
	this->menuViewer = new MenuViewer(textures,defaultFont);


	view = sf::View({ 0.f,0.f }, { (float)window->getSize().x, (float)window->getSize().y });
	view.setCenter({ float(this->W_WIDTH / 2),float(this->W_HEIGHT / 2) });
	this->window->setView(view);
}

GUI::~GUI()
{
	delete this->window;

	if (menuViewer != nullptr)
	{
		delete menuViewer;
	}

	if (sortingViewer != nullptr)
	{
		delete sortingViewer;
	}

	if (pathFindViewer != nullptr)
	{
		delete pathFindViewer;
	}

	deleteTextures();
}

//Accesors

const bool GUI::running() const
{
	return this->window->isOpen();
}

sf::RenderWindow* GUI::getWindow() {
	return this->window;
}

void GUI::eventsReset() {
	mouseClickEvent = false;
	mouseReleasedEvent = false;
	keyPressed = sf::Keyboard::Key::Unknown;
}

void GUI::updateMousePosition() {

	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = window->mapPixelToCoords(this->mousePosWindow);
}

void GUI::pollEvents()
{

	//event polling
	while (window->pollEvent(ev))
	{
		sf::Vector2f newSize;
		sf::Vector2f ratio;

		switch (ev.type)
		{

		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed: keyPressed = ev.key.code;
	
			break;
		case sf::Event::MouseButtonPressed:mouseClickEvent = true;
			break;
		case sf::Event::MouseButtonReleased:mouseReleasedEvent = true;
			break;
		case sf::Event::Resized:

		// resize my view

		newSize =sf::Vector2f(window->getSize());
		ratio = sf::Vector2f(newSize.x / W_WIDTH, newSize.y / W_HEIGHT);

		view.setSize({
				static_cast<float>(ev.size.width),
				static_cast<float>(ev.size.height)
		});

		view.zoom(1.f / std::min(ratio.x, ratio.y));
		window->setView(view);
		
		mousePosView = window->mapPixelToCoords(mousePosWindow);

		this->lastWindowSize = window->getSize();
		/*
		view = sf::View({ 0.f, 0.f },sf::Vector2f(ev.size.width, ev.size.height));
		view.setCenter({ float(ev.size.width / 2),float(ev.size.height / 2) });
		window->setView(view);
		*/
			break;
		default:
			break;
		}
	}
}

void GUI::createSortingViewer() {
	if (sortingViewer != nullptr)
	{
		delete sortingViewer;
	}
	controller.generateSimulation(numElements, 200, sortingType);
	sortingViewer = new SortingViewer(controller.getSimulation(), textures, defaultFont,numElements,sortingType);
}

void GUI::createPathFindViewer() {
	if (pathFindViewer != nullptr)
	{
		delete pathFindViewer;
	}
	pathFindViewer = new PathFindViewer(nullptr, textures, defaultFont, pathFindType);
}

void GUI::updateMenu() {


	this->menuViewer->update(mouseClickEvent, mousePosView);

	if (mouseClickEvent)
	{
		if (this->menuViewer->sortingAction)
		{
			state = ProgramState::in_sorting;
			menuViewer->sortingAction = false;
			createSortingViewer();
		}
		else if (this->menuViewer->pathFindAction)
		{
			state = ProgramState::in_pathfind;
			menuViewer->pathFindAction = false;
			createPathFindViewer();
		}
		else if (this->menuViewer->exitAction)
		{
			window->close();
		}
	}
}

void GUI::updateSortingViewer() {

	bool keyEvent = keyPressed == sf::Keyboard::Key::Unknown ? false : true;

	SortingAction action = SortingAction::None;

	if (keyEvent && (keyPressed == sf::Keyboard::Key::D || keyPressed == sf::Keyboard::Key::Right))
	{
		action = SortingAction::Forward;
	}
	else if (keyEvent && (keyPressed == sf::Keyboard::Key::A || keyPressed == sf::Keyboard::Key::Left))
	{
		action = SortingAction::Backward;
	}
	else if (keyEvent && (keyPressed == sf::Keyboard::Key::Space))
	{
		action = SortingAction::Pause;
	}
	else if (sortingViewer->pauseAction)
	{
		action = SortingAction::Pause;
	}
	else if (sortingViewer->forwardAction)
	{
		action = SortingAction::Forward;
	}
	else if (sortingViewer->backwardAction)
	{
		action = SortingAction::Backward;
	}
	else if (sortingViewer->playForwardAction)
	{
		action = SortingAction::PlayForward;
	}
	else if (sortingViewer->playBackwardAction)
	{
		action = SortingAction::PlayBackward;
	}

	sortingViewer->update(action);


	sortingViewer->update(mouseClickEvent, mouseReleasedEvent, mousePosView);

	/*if (mouseClickEvent || mouseReleasedEvent || sortingViewer->mousePressedAction)
	{
		sortingViewer->update(mouseClickEvent, mouseReleasedEvent, sf::Vector2f(mousePosWindow));
	}*/

	if (sortingViewer->exitAction)
	{

		sortingViewer->exitAction = false;
		delete sortingViewer;
		sortingViewer = nullptr;
		state = ProgramState::in_menu;
	}
	else if (sortingViewer->resetAction)
	{
		numElements = sortingViewer->getNumElements();
		sortingType = sortingViewer->getSortType();
		sortingViewer->resetAction = false;
		createSortingViewer();
	}
}

void GUI::updatePathFindViewer() {


	bool keyEvent = keyPressed == sf::Keyboard::Key::Unknown ? false : true;

	PathFindAction action = PathFindAction::None;

	if (keyEvent && (keyPressed == sf::Keyboard::Key::D || keyPressed == sf::Keyboard::Key::Right))
	{
		action = PathFindAction::Forward;
	}
	else if (keyEvent && (keyPressed == sf::Keyboard::Key::A || keyPressed == sf::Keyboard::Key::Left))
	{
		action = PathFindAction::Backward;
	}
	else if (keyEvent && (keyPressed == sf::Keyboard::Key::Space))
	{
		action = PathFindAction::Pause;
	}
	else if (pathFindViewer->pauseAction)
	{
		action = PathFindAction::Pause;
	}
	else if (pathFindViewer->forwardAction)
	{
		action = PathFindAction::Forward;
	}
	else if (pathFindViewer->backwardAction)
	{
		action = PathFindAction::Backward;
	}
	else if (pathFindViewer->playForwardAction)
	{
		action = PathFindAction::PlayForward;
	}
	else if (pathFindViewer->playBackwardAction)
	{
		action = PathFindAction::PlayBackward;
	}


	pathFindViewer->update(action);
	pathFindViewer->update(mouseClickEvent, mouseReleasedEvent, mousePosView);
	/*if (mouseClickEvent || mouseReleasedEvent || pathFindViewer->mousePressedAction)
	{
		pathFindViewer->update(mouseClickEvent, mouseReleasedEvent, sf::Vector2f(mousePosWindow));
	}*/

	if (pathFindViewer->exitAction)
	{

		pathFindViewer->exitAction = false;
		delete pathFindViewer;
		pathFindViewer = nullptr;
		state = ProgramState::in_menu;
	}
	else if (pathFindViewer->resetAction)
	{
		pathFindType = pathFindViewer->getPathFindType();
		pathFindViewer->resetAction = false;
		createPathFindViewer();
	}
}

void GUI::update() {

	this->pollEvents();
	this->updateMousePosition();
	

	if (state == ProgramState::in_menu)
	{
		updateMenu();
	}
	else if (state == ProgramState::in_sorting)
	{
		updateSortingViewer();
	}
	else if (state == ProgramState::in_pathfind)
	{
		updatePathFindViewer();
	}

	eventsReset();
}

void GUI::render() {


	this->window->clear(sf::Color(236, 224, 217));

	
	if (state == ProgramState::in_menu)
	{
		this->menuViewer->render(window);
	}
	else if (state == ProgramState::in_sorting)
	{
		this->sortingViewer->render(window);
	}
	else if (state == ProgramState::in_pathfind)
	{
		this->pathFindViewer->render(window);
	}
	

	this->window->display();

}