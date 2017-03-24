#include "Pathing.h"
#include <iostream>

Path FindPath(Map& map, Node start, Node goal, int max_distance) {
	/* ==============================================
		STEP ZERO:
		Perform quick error checks to make sure there
		are no invalid arguments.
	==============================================  */
	bool error = false;

	// First make sure the start and goal are actually apart
	if (start == goal) {
		error = true;
	}

	/* Quick check that the start and goal are not covered up by
	   unpathable tiles. This will prevent unncessary attempts
	   at solving a path. */
	vector<Node> err_nodes;
	err_nodes.push_back(start);
	err_nodes.push_back(goal);

	// Loop trough both locations
	for (size_t i = 0; i < err_nodes.size(); i++) {
		// Count unpathable tiles around this node
		int error_unpathable = 0;

		// Scan 3x3 grid
		for (int yc = err_nodes[i].y - 1; yc <= err_nodes[i].y + 1; ++yc) {
			for (int xc = err_nodes[i].x - 1; xc <= err_nodes[i].x + 1; ++xc) {
				// If a legal index
				if (xc >= 0 && xc < Map::MAP_SIZE_X &&
					yc >= 0 && yc < Map::MAP_SIZE_Y) {

					// If not the center
					if (!(xc == err_nodes[i].x && yc == err_nodes[i].y)) {
						if (!TileIsPathable(map.tile[xc][yc])) {
							// Unpathable tile found
							error_unpathable++;
						}
					} else {
						// Check if center is blocked
						if (!TileIsPathable(map.tile[xc][yc]) &&
							!(map.tile[xc][yc] >= 28 && map.tile[xc][yc] <= 31) &&
							!(map.tile[xc][yc] >= 33 && map.tile[xc][yc] <= 36)) {
							error = true;
						}
					}
				} else {
					// Increment unpathable squares count
					error_unpathable++;

					// Check if center is illegal
					if (xc == err_nodes[i].x && yc == err_nodes[i].y) {
						error = true;
					}
				}
			}
		}
		
		// If all 8 squares are blocking our pathway
		if (error_unpathable >= 8) {
			error = true;
		}

	}


	// Return an empty path if any error occured
	if (error) return Path();

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

	/* A vector that stores vectors of nodes (coordinates).
	   The initial vector index represents a distance.
	   Example: mark[3][0] would access the first (of many)
	   points that are 3 squares away from goal. */
	vector<vector<Node>> mark;

	// Add the goal node(s) to marks
	vector<Node> goal_marks;
	goal_marks.push_back(goal);
	mark.push_back(goal_marks);
	int distance = 1;

	// Mark goal as 0 (no distance from goal)
	matrix[goal.x][goal.y] = 0;

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
			int x = current_node.x;
			int y = current_node.y;

			// Scan the 3x3 grid around current location
			for (int yc = y - 1; yc <= y + 1; ++yc) {
				for (int xc = x - 1; xc <= x + 1; ++xc) {
					// If a legal index
					if (xc >= 0 && xc < Map::MAP_SIZE_X &&
						yc >= 0 && yc < Map::MAP_SIZE_Y) {

						// If not explored
						if (matrix[xc][yc] == NodeState::Unexplored) {

							// If pathable
							if (TileIsPathable(map.tile[xc][yc])) {

								// Mark it
								matrix[xc][yc] = distance;
								this_distance_marks.push_back(Node(xc, yc, &mark[distance - 1][i]));
							} else {

								// Mark as unpathable
								matrix[xc][yc] = NodeState::Unpathable;
							}
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

	/* Clear the last distance in vector since we don't need
	   it to connect to the start */
	mark.pop_back();
	
	/* ==============================================
		STEP TWO:
		Figure out the best path using our distance markings.
	==============================================  */
	Path result;
	result.node.push_back(start);

	// Loop trough all marked distances
	for (size_t i = mark.size() - 1; i > 0; --i) {
		// The node we're working with
		Node current_node = result.node.back();

		// The coordinates we'll explore around
		int x = current_node.x;
		int y = current_node.y;

		// Choose next node
		Node best_node;
		bool straight = false;
		bool found_a_node = false;
		for (size_t j = 0; j < mark[i].size(); ++j) {
			Node next_node = mark[i][j];

			// Quick check if this node is connected
			if (current_node.last_node != NULL)
				if (*current_node.last_node != next_node) break;

			// First try a straight turn, then diagonal
			if (next_node.x == x - 1 && next_node.y == y ||
				next_node.x == x + 1 && next_node.y == y ||
				next_node.x == x && next_node.y == y - 1 ||
				next_node.x == x && next_node.y == y + 1) {

				// Set this node as the best suiting one
				best_node = next_node;
				straight = true;
				found_a_node = true;
				break;
			} else
			if (next_node.x == x - 1 && next_node.y == y - 1 ||
				next_node.x == x + 1 && next_node.y == y - 1 ||
				next_node.x == x - 1 && next_node.y == y + 1 ||
				next_node.x == x + 1 && next_node.y == y + 1) {

				// Set this node as the best suiting one\
				// But straight > diagonal
				if (!straight) {
					best_node = next_node;
					found_a_node = true;
				}
			}
		}
		if(found_a_node) result.node.push_back(best_node);
	}

	// Finally goal
	result.node.push_back(goal);

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
	int x = cord.x;
	int y = cord.y;
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
		result.x = x * Map::TILE_SIZE + (x_offset);
		result.y = y * Map::TILE_SIZE + (y_offset);
		return result;
	} else {
		// Return the center
		Node result;
		result.x = x * Map::TILE_SIZE + (Map::TILE_SIZE / 2);
		result.y = y * Map::TILE_SIZE + (Map::TILE_SIZE / 2);
		return result;
	}
}