#pragma once
#include <vector>
#include "Map.h"

using namespace std;
typedef pair<int, int> Node;

enum NodeState { Unexplored = -2, Blocked = -1 };

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