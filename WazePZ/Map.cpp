#include "Map.h"

void Map::renderStartEnd(){
	int rad = 10;

	CircleShape nodeCircle(rad);
	nodeCircle.setFillColor(Color(0, 255, 0, 128));

	if (p1.x != -1) {
		nodeCircle.setPosition(p1.x - rad, p1.y - rad);
		mapWindow->draw(nodeCircle);
	}
	if (p2.x != -1) {
		nodeCircle.setPosition(p2.x - rad, p2.y - rad);
		mapWindow->draw(nodeCircle);
	}
}

void Map::renderNodes(){
	list<Vector2i> allCoords = map.getAllNodeCoords();

	int rad = 10;

	CircleShape nodeCircle(rad);
	nodeCircle.setFillColor(Color(255, 0, 0, 128));

	for (Vector2i XY : allCoords) {
		nodeCircle.setPosition(XY.x - rad, XY.y - rad);
		mapWindow->draw(nodeCircle);
	}
}

bool Map::isPressingButton(Sprite button){
	if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2i point = Mouse::getPosition(*mapWindow);
		bool isInX = point.x >= button.getPosition().x && point.x <= button.getPosition().x + button.getScale().x;
		bool isInY = point.y >= button.getPosition().y && point.y <= button.getPosition().y + button.getScale().y;
		return isInX && isInY;
	}
	return false;
}


void Map::chargeFiles(){
	inter.loadFromFile("textures/interface.png");
	buttonTexture.loadFromFile("textures/Button.png");
	txt.loadFromFile("fonts/Cabin-Bold.ttf");
	interface.setTexture(inter);
	startSearch.setTexture(buttonTexture);

	startSearch.setScale(0.6, 0.6);
	startSearch.setPosition(626, 50);
	initRoute.setScale(0.6, 0.6);
	initRoute.setTexture(buttonTexture);
	initRoute.setPosition(626, 130);
	cancelRoute.setScale(0.6, 0.6);
	cancelRoute.setTexture(buttonTexture);
	cancelRoute.setPosition(626, 210);

	text1.setFont(txt);
	text1.setFillColor(Color(42, 213, 255, 255));
	text1.setString("Buscar ruta");
	text1.setCharacterSize(18);
	text1.setPosition(658, 68);

	text2.setFont(txt);
	text2.setFillColor(Color(42, 213, 255, 255));
	text2.setString("Iniciar ruta");
	text2.setCharacterSize(18);
	text2.setPosition(662, 148);

	text3.setFont(txt);
	text3.setFillColor(Color(42, 213, 255, 255));
	text3.setString("Cancelar");
	text3.setCharacterSize(18);
	text3.setPosition(670, 228);
}

Map::Map(){
	mapEvent = Event();
	this->mapWindow = new RenderWindow(VideoMode(800, 600), "Mapa de PZ", Style::Close);
	this->mapWindow->setFramerateLimit(25);
	chargeFiles();
	leftMouse = true;
	leftMouseDebug = true;
	pKey = true;
	rKey = true;
	p1 = Vector2i(-1, 0);
	p2 = Vector2i(-1, 0);
}

Map::~Map(){

	delete this->mapWindow;
}

void Map::render() {
	mapWindow->draw(interface);
	if (debugMode) { renderNodes(); }
	renderStartEnd();

	mapWindow->draw(startSearch);
	mapWindow->draw(initRoute);
	mapWindow->draw(cancelRoute);

	mapWindow->draw(text1);
	mapWindow->draw(text2);
	mapWindow->draw(text3);

	mapWindow->display();
	this_thread::sleep_for(chrono::milliseconds(40));
}

void Map::update(){

	if (Keyboard::isKeyPressed(Keyboard::P) && pKey) {			// Activar/Desactivar DebugMode
		if (debugMode) {
			debugMode = false;
		}
		else {
			debugMode = true;
		}
		pKey = false;
	}
	if (!Keyboard::isKeyPressed(Keyboard::P)) {
		pKey = true;
	}

	if (Mouse::isButtonPressed(Mouse::Left) && leftMouse) {
		leftMouse = false;
		if (isAbodeNode(Mouse::getPosition(*mapWindow))) {
			node* clickedNode = map.getAbodeNode(Mouse::getPosition(*mapWindow), 10);
			cout << "\n[INFO]\n ID: " << clickedNode->id << ", X: " << clickedNode->x << ", Y: " << clickedNode->y << ", Name: " << clickedNode->name << "\n";

			cout << "[Conexiones]: ";
			if (clickedNode->adj.empty()) {
				cout << "No hay nodos conectados." << endl;
			}
			else {
				for (node* adjNode : clickedNode->adj) {
					cout << "[ID: " << adjNode->id << ", Name: " << adjNode->name << "] ";
				}
				cout << endl;
			}

			if (p1.x != clickedNode->x && p1.y != clickedNode->y) {
				p2 = p1;
				p1.x = clickedNode->x;
				p1.y = clickedNode->y;
			}



			if (isPressingButton(startSearch)) {
				currentRoute = map.findShortestRoute(map.getAbodeNode(p2, 10), map.getAbodeNode(p1, 10));
				cout << "Recorrido: ";
				for (node* node : currentRoute) {
					cout << node->id << "->";
				}
			}
			if (isPressingButton(initRoute)) {
				//hacer recorrido de la ruta
			}
		}
	}
	if (!Mouse::isButtonPressed(Mouse::Left)) {
		leftMouse = true;
	}
}

void Map::updateDebug() {
	if (!debugMode) {
		return;
	}

	if (Mouse::isButtonPressed(Mouse::Left) && leftMouseDebug) {
		bool isInX = Mouse::getPosition(*mapWindow).x <= mapWindow->getSize().x - 192 && Mouse::getPosition(*mapWindow).x >= 0;
		bool isInY = Mouse::getPosition(*mapWindow).y <= mapWindow->getSize().y && Mouse::getPosition(*mapWindow).y >= 0;
		if (isInX && isInY && debugMode && !isAbodeNode(Vector2i(Mouse::getPosition(*mapWindow).x, Mouse::getPosition(*mapWindow).y))) {
			cout << "[" << Mouse::getPosition(*mapWindow).x << ", " << Mouse::getPosition(*mapWindow).y << "]\n";
			//map.addNode(map.genNodeID(), Mouse::getPosition(*mapWindow).x, Mouse::getPosition(*mapWindow).y, "");
		}

		leftMouseDebug = false;
	}
	if (!Mouse::isButtonPressed(Mouse::Left)) {
		leftMouseDebug = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::R) && rKey) {
		map.reloadNodes();
		p1 = Vector2i(-1, 0);
		p2 = Vector2i(-1, 0);
		rKey = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		rKey = true;
	}
}

bool Map::isAbodeNode(Vector2i point) {

	list<Vector2i> allCoords = map.getAllNodeCoords();

	for (Vector2i current : allCoords) {
		if ((point.x > current.x - 10 && point.x < current.x + 10) && (point.y > current.y - 10 && point.y < current.y + 10)) {
			return true;
		}
	}
	return false;
}

bool Map::isRunning(){
	
	while (mapWindow->pollEvent(mapEvent)) {
		if (mapEvent.type == Event::Closed) {
			mapWindow->close();
		}
	}

	return this->mapWindow->isOpen();
}