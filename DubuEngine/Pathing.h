#pragma once
#include <vector>
#include "Map.h"

using namespace std;
//typedef pair<int, int> Node;
class Node {
public:
	bool operator==(const Node& n) { return (x == n.x && y == n.y); }
	bool operator!=(const Node& n) { return (!(x == n.x && y == n.y)); }
	Node() {}
	Node(int new_x, int new_y) : x(new_x), y(new_y) {}
	Node(int new_x, int new_y, Node* new_last_node) : x(new_x), y(new_y), last_node(new_last_node) {}
	int x;
	int y;
	Node* last_node = NULL;
};

enum NodeState { Unexplored = -2, Unpathable = -1 };

struct Path {
	// Reset
	void Reset() { successful = false; current_node = 0; node.clear(); }

	// Nodes for pathway
	vector<Node> node;

	// If this path is succesful
	bool successful = false;

	// Can be used by AI, etc. to count the currently followed node
	int current_node = 0;
};

// Returns the fastest path from start to goal
Path FindPath(Map& map, Node start, Node goal, int max_distance = 20);

// Returns a node converted to real coordinates
Node ConvertToReal(Map& map, Node cord);