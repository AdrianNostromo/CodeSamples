#pragma once

class VoxelSides {
public:
	// New side names. Recommended.
	static const int xMin;
	static const int xMax;

	static const int yMin;
	static const int yMax;

	static const int zMin;
	static const int zMax;
	
	// New side names. Recommended.
	static const int BitGroup_xMin;
	static const int BitGroup_xMax;

	static const int BitGroup_yMin;
	static const int BitGroup_yMax;

	static const int BitGroup_zMin;
	static const int BitGroup_zMax;

	// Old names. Not recommended.
	static const int Front;
	static const int Back;

	static const int Left;
	static const int Right;

	static const int Up;
	static const int Down;
};
