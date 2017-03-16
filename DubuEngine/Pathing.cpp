#include "Pathing.h"
#include <iostream>

Path FindPath(Map& map, Node start, Node goal, int max_distance) {
	/* ==============================================
		STEP ONE:
		Create a matrix of the map and start marking the distances
		from goal to each square.
	==============================================  */
	// Declare some constants
	int matrix[Map::MAP_SIZE_X][Map::MAP_SIZE_Y];
	for (int x = 0; x < Map::MAP_SIZE_X; ++x) {
		for (int y = 0; y < Map::MAP_SIZE_Y; ++y) {
			// Mark all unexplored
			matrix[x][y] = NodeState::Unexplored;
		}
	}

	// A vector that stores vectors of nodes (coordinates).
	// The initial vector index represents a distance.
	// Example: mark[3][0] would access the first (of many) points that are 3 squares away from goal.
	vector<vector<Node>> mark;

	// Add the goal node(s) to marks
	vector<Node> goal_marks;
	goal_marks.push_back(goal);
	mark.push_back(goal_marks);
	int distance = 1;

	// Mark goal as 0 (no distance from goal)
	matrix[goal.first][goal.second] = 0;

	// Loop till the start is found or till we break
	bool marked_start = false;
	while (!marked_start) {
		// The vector of nodes for this distance
		vector<Node> this_distance_marks;

		// If last vector is empty or we maxed out or boundary, we failed to mark the start
		if (mark[distance - 1].size() == 0 || distance > max_distance) {
			// Break loop
			break;
		}

		// Loop trough previous distance nodes
		for (size_t i = 0; i < mark[distance - 1].size(); ++i) {
			// The node we're working with
			Node current_node = mark[distance - 1][i];

			// The coordinates we'll explore around
			int x = current_node.first;
			int y = current_node.second;

			// Left
			if (x > 0) {
				if (matrix[x - 1][y] == NodeState::Unexplored) {
					if (TileIsPathable(map.tile[x - 1][y])) {
						// Mark it
						matrix[x - 1][y] = distance;
						this_distance_marks.push_back(make_pair(x - 1, y));
					}
				}
			}

			// Right
			if (x < Map::MAP_SIZE_X - 1) {
				if (matrix[x + 1][y] == NodeState::Unexplored) {
					if (TileIsPathable(map.tile[x + 1][y])) {
						// Mark it
						matrix[x + 1][y] = distance;
						this_distance_marks.push_back(make_pair(x + 1, y));
					}
				}
			}

			// Above
			if (y > 0) {
				if (matrix[x][y - 1] == NodeState::Unexplored) {
					if (TileIsPathable(map.tile[x][y - 1])) {
						// Mark it
						matrix[x][y - 1] = distance;
						this_distance_marks.push_back(make_pair(x, y - 1));
					}
				}
				
				// Top-left
				if (x > 0) {
					if (matrix[x - 1][y - 1] == NodeState::Unexplored) {
						if (TileIsPathable(map.tile[x - 1][y - 1])) {
							// Mark it
							matrix[x - 1][y - 1] = distance;
							this_distance_marks.push_back(make_pair(x - 1, y - 1));
						}
					}
				}

				// Top-right
				if (x < Map::MAP_SIZE_X - 1) {
					if (matrix[x + 1][y - 1] == NodeState::Unexplored) {
						if (TileIsPathable(map.tile[x + 1][y - 1])) {
							// Mark it
							matrix[x + 1][y - 1] = distance;
							this_distance_marks.push_back(make_pair(x + 1, y - 1));
						}
					}
				}
			}

			// Bellow
			if (y < Map::MAP_SIZE_Y - 1) {
				if (matrix[x][y + 1] == NodeState::Unexplored) {
					if (TileIsPathable(map.tile[x][y + 1])) {
						// Mark it
						matrix[x][y + 1] = distance;
						this_distance_marks.push_back(make_pair(x, y + 1));
					}
				}

				// Bottm-left
				if (x > 0) {
					if (matrix[x - 1][y + 1] == NodeState::Unexplored) {
						if (TileIsPathable(map.tile[x - 1][y + 1])) {
							// Mark it
							matrix[x - 1][y + 1] = distance;
							this_distance_marks.push_back(make_pair(x - 1, y + 1));
						}
					}
				}

				// Bottom-right
				if (x < Map::MAP_SIZE_X - 1) {
					if (matrix[x + 1][y + 1] == NodeState::Unexplored) {
						if (TileIsPathable(map.tile[x + 1][y + 1])) {
							// Mark it
							matrix[x + 1][y + 1] = distance;
							this_distance_marks.push_back(make_pair(x + 1, y + 1));
						}
					}
				}
			}

			// Quick check if we marked the start
			for (size_t j = 0; j < this_distance_marks.size(); ++j) {
				if (this_distance_marks[j] == start) marked_start = true;
			}
		}

		// Add these marks to the vector
		mark.push_back(this_distance_marks);

		// Increment distance for next loop
		++distance;
	}
	
	/* ==============================================
		STEP TWO:
		Figure out the best path using our distance markings.
	==============================================  */
	Path result;
	result.node.push_back(start);
	for (size_t i = mark.size() - 1; i > 0; --i) {
		// The node we're working with
		Node current_node = result.node.back();

		// The coordinates we'll explore around
		int x = current_node.first;
		int y = current_node.second;

		// Choose next node
		for (size_t j = 0; j < mark[i].size(); ++j) {
			Node next_node = mark[i][j];
			// First try a straight turn, then diagonal
			if (next_node.first == x - 1 && next_node.second == y ||
				next_node.first == x + 1 && next_node.second == y ||
				next_node.first == x && next_node.second == y - 1 ||
				next_node.first == x && next_node.second == y + 1) {
				result.node.push_back(next_node);
				break;
			} else
			if (next_node.first == x - 1 && next_node.second == y - 1 ||
				next_node.first == x + 1 && next_node.second == y - 1 ||
				next_node.first == x - 1 && next_node.second == y + 1 ||
				next_node.first == x + 1 && next_node.second == y + 1) {
				result.node.push_back(next_node);
				break;
			}
		}
	}

	// Convert result to real coordinates
	for (size_t i = 0; i < result.node.size(); ++i) {
		result.node[i] = ConvertToReal(map, result.node[i]);
	}

	// Return resulting path
	result.successful = true;
	result.current_node = 0;
	return result;
}

Node ConvertToReal(Map& map, Node cord) {
	int x = cord.first;
	int y = cord.second;
	int tile_id = map.tile[x][y];
	if (TileIsWater(tile_id)) {
		// Offsets
		int x_offset = 0;
		int y_offset = 0;

		// Shift for specific sprites
		if (tile_id == 28 || tile_id == 35) x_offset += Map::TILE_SIZE / 2;
		if (tile_id == 30 || tile_id == 33 || tile_id == 36) x_offset += Map::TILE_SIZE;
		if (tile_id == 30 || tile_id == 31 || tile_id == 33) y_offset += Map::TILE_SIZE / 2;
		if (tile_id == 34 || tile_id == 35 || tile_id == 36) y_offset += Map::TILE_SIZE;

		// Return proper coordinate
		Node result;
		result.first = x * Map::TILE_SIZE + (x_offset);
		result.second = y * Map::TILE_SIZE + (y_offset);
		return result;
	} else {
		// Return the center
		Node result;
		result.first = x * Map::TILE_SIZE + (Map::TILE_SIZE / 2);
		result.second = y * Map::TILE_SIZE + (Map::TILE_SIZE / 2);
		return result;
	}
}