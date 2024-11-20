#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <cstdlib>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "Button.h"

struct node {
	int id;
	string name;
	int x;
	int y;
	list<node*> adj;
	list<int> heights;
	list<string> events;
	node(int ID, int X, int Y, string Name) : id(ID), x(X), y(Y), name(Name) {}
};

class Graph {
private:
	
	list<node*> routes;

	void loadNodes();
	void linkNodes(int);
	node* getNode2(int);

	void printNodes();
	node* getNode(int);

public:
	Graph();
	int genNodeID();
	list<node*> findShortestRoute(node*, node*);
	void reloadNodes();
	void addNode(int, int, int, string);
	void addNodesInfo();
	void searchRoutes(int);
	void randomEvents();
	node* getAbodeNode(Vector2i, int);
	list<Vector2i> getAllNodeCoords();
};
