#pragma once
#include <vector>
#include "Map.h"

using namespace std;
//typedef pair<int, int> Node;
class Node {
public:
	bool operator==(const Node& n) { return (x == n.x && y == n.y); }
	Node() {}
	Node(int new_x, int new_y) : x(new_x), y(new_y) {}
	int x;
	int y;
};

enum NodeState { Unexplored = -2, Unpathable = -1 };

struct Path {
	// Nodes for pathway
	vector<Node> node;

	// If this path is succesful
	bool successful = false;

	// Can be used by AI, etc. to count the currently followed node
	int current_node = 0;
};

// Returns the fastest path from start to goal
Path FindPath(Map& map, Node start, Node goal, int max_distance = 100);

// Returns a node converted to real coordinates
Node ConvertToReal(Map& map, Node cord);