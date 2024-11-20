#include "Graph.h"

Graph::Graph(){
	loadNodes();
	printNodes();
	srand(time(NULL));
}

int Graph::genNodeID() {
	int current = 0;
	for (node* currentNode : routes) {
		if (currentNode->id != current) {
			return current;
		}
		current++;
	}
	return current;
}

void Graph::reloadNodes(){
	system("cls");
	loadNodes();
	printNodes();
}

void Graph::loadNodes() {
	routes.clear();
	ifstream routesFile("map/rpos.txt");
	string rNode;
	if (!routesFile.is_open()) {
		return;
	}

	while (getline(routesFile, rNode)) {
		vector<string> vars;
		stringstream ss(rNode);
		string segment;

		while (getline(ss, segment, '_')) {
			vars.push_back(segment);
		}
		int id = stoi(vars[0].substr(2, vars[0].size()));
		int x = stoi(vars[1].substr(2, vars[1].size()));
		int y = stoi(vars[2].substr(2, vars[2].size()));
		string name = vars[3].substr(2, vars[3].size());
		routes.push_back(new node(id, x, y, name));


	}

	for (node* current : routes)
	{
		linkNodes(current->id);
	}
}

void Graph::linkNodes(int nodeId) {
	ifstream file("map/rcon.txt");
	if (!file.is_open()) {
		cerr << "Error: No se pudo abrir el archivo rcon.txt" << endl;
		return;
	}

	string line;
	bool found = false;

	while (getline(file, line)) {
		stringstream ss(line);
		int id;
		char colon;

		ss >> id >> colon; // Leer el ID del nodo y descartar el ':'

		if (id == nodeId) {
			found = true;

			node* currentNode = getNode2(nodeId);
			if (!currentNode) {
				cerr << "Error: Nodo con ID " << nodeId << " no encontrado en la lista de nodos." << endl;
				break;
			}

			int adjacentId;
			while (ss >> adjacentId) {
				node* adjacentNode = getNode2(adjacentId);
				if (adjacentNode) {
					currentNode->adj.push_back(adjacentNode);
				}
				else {
					cerr << "Advertencia: Nodo adyacente con ID " << adjacentId << " no encontrado." << endl;
				}
			}
			break; // Detener el bucle después de encontrar el nodo
		}
	}

	if (!found) {
		cerr << "Error: Nodo con ID " << nodeId << " no encontrado en el archivo rcon.txt." << endl;
	}

	file.close();
	cout << "Las conexiones para el nodo " << nodeId << " se han cargado correctamente." << endl;
}

node* Graph::getNode2(int id) {
	for (node* n : routes) {
		if (n->id == id) {
			return n;
		}
	}
	return nullptr;
}

void Graph::printNodes(){
	for (node* current : routes) {
		cout << "\nID: " << current->id << ", X: " << current->x << ", Y: " << current->y << ", Name: " << current->name << "\n";
	}
}

node* Graph::getNode(int number){
	int num = 0;
	for (node* current : routes) {
		if (num != number) {
			num++;
		}
		else {
			return current;
		}
	}
	return nullptr;
}

list<node*> Graph::findShortestRoute(node* start, node* end) {
	list<node*> routes;

	if (!start || !end) {
		cerr << "Error: Uno o ambos nodos son inválidos." << endl;
		return routes;
	}

	// Mapas para las distancias y los predecesores
	unordered_map<int, int> distances; // Distancia mínima a cada nodo
	unordered_map<int, int> parent;    // Para reconstruir el camino

	// Inicializar distancias a infinito y padre a -1
	for (node* n : this->routes) { // Usar la lista global de nodos
		distances[n->id] = numeric_limits<int>::max();
		parent[n->id] = -1;
	}

	// Cola de prioridad para nodos a procesar (distancia, nodo)
	priority_queue<pair<int, node*>, vector<pair<int, node*>>, greater<>> pq;
	distances[start->id] = 0; // Distancia al nodo inicial es 0
	pq.push({ 0, start });

	while (!pq.empty()) {
		auto topPair = pq.top(); // Extraer el par desde la cola
		int currentDist = topPair.first;
		node* currentNode = topPair.second;
		pq.pop();

		// Si llegamos al nodo objetivo, terminar la búsqueda
		if (currentNode == end) break;

		// Explorar vecinos
		for (node* neighbor : currentNode->adj) {
			int newDist = currentDist + 1; // Cada salto cuenta como +1
			if (newDist < distances[neighbor->id]) {
				distances[neighbor->id] = newDist;
				parent[neighbor->id] = currentNode->id;
				pq.push({ newDist, neighbor });
			}
		}
	}

	// Si no se encontró una ruta, regresar vacío
	if (distances[end->id] == numeric_limits<int>::max()) {
		cout << "No se encontró una ruta entre el nodo " << start->id << " y el nodo " << end->id << "." << endl;
		return routes;
	}

	// Reconstruir la ruta
	vector<int> path;
	for (int at = end->id; at != -1; at = parent[at]) {
		path.push_back(at);
	}
	reverse(path.begin(), path.end());

	// Convertir los IDs del camino en punteros a nodos y llenar la lista `routes`
	for (int id : path) {
		for (node* n : this->routes) {
			if (n->id == id) {
				routes.push_back(n);
				break;
			}
		}
	}

	// Mostrar la ruta más corta
	/*cout << "\n--- Ruta más corta encontrada ---" << endl;
	cout << "Origen: Nodo " << start->id << " (" << start->name << ")" << endl;
	cout << "Destino: Nodo " << end->id << " (" << end->name << ")" << endl;
	cout << "Recorrido: ";*/
	for (node* n : routes) {
		cout << n->id;
		if (n != routes.back()) {
			cout << " -> ";
		}
	}
	cout << endl;

	return routes;
}

void Graph::addNode(int id, int x, int y, string name){
	routes.push_back(new node(id, x, y, name));

	ofstream routes("map/rpos.txt", ios::app);
	if (routes.is_open()) {
		routes << "i:" << id << " x:" << x << " y:" << y << " n:" << name << " " << endl;
		routes.close();
	}
}

void Graph::addNodesInfo(){

}

void Graph::searchRoutes(int cant){

}

void Graph::randomEvents(){

	for (node* current : routes) {
		current->events.clear();
	}

	string events[5] = { "Cerrado", "Accidente", "Normal", "Moderado", "Lento" };

	int randomNode, randomConn, randomEvent;

	for (int r = 0; r < 20; r++) {
		randomNode = rand() % routes.size();
		node* current = getNode(randomNode);
		
		randomConn = rand() % current->adj.size();
		for (int s = 0; s < current->adj.size(); s++) {
			if (s != randomConn) {
				randomEvent = rand() % 5;
				current->events.push_back(events[randomEvent]);
			}
			else {
				current->events.push_back("");
			}
		}
	}
}

node* Graph::getAbodeNode(Vector2i point, int rad){

	for (node* node : routes) {
		if ((point.x > node->x - rad && point.x < node->x + rad) && (point.y > node->y - rad && point.y < node->y + rad)) {
			return node;
		}
	}

	return nullptr;
}

list<Vector2i> Graph::getAllNodeCoords(){
	list<Vector2i> coords;
	for (node* current : routes) {
		coords.push_back(Vector2i(current->x, current->y));
	}
	return coords;
}