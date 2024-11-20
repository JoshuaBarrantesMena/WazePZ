#pragma once
#include "Graph.h"
using namespace sf;

class Map {
private:
	bool debugMode = false;					//todo el testeo grafico depende de esta variable
	Graph map;

	RenderWindow* mapWindow;
	Event mapEvent;
	Texture inter, buttonTexture;
	Sprite interface, startSearch, initRoute, cancelRoute;
	Text text1, text2, text3;
	Font txt;

	list<node*> currentRoute;

	bool leftMouse;
	bool leftMouseDebug; //debug key
	bool pKey;			 //debug key
	bool rKey;

	Vector2i p1, p2;

	void renderStartEnd();
	void renderNodes();
	bool isPressingButton(Sprite);
	void chargeFiles();

public:
	Map();
	~Map();

	void render();
	void update();
	void updateDebug();

	bool isAbodeNode(Vector2i);
	bool isRunning();
};