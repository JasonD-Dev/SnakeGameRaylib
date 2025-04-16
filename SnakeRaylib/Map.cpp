#include "Map.h"

Map::Map() {
	for (int i = 0; i < gridHeight; i++) {
		for (int j = 0; j < gridWidth; j++) {
			grid[i][j] = ' ';
		}
	}
}