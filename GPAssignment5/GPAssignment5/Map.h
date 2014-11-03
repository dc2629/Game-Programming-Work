#pragma once
#include<sstream>
#include<vector>
#include<string>
#include<iostream>
#include <fstream>
#include<algorithm>

using namespace std;

class Map{
public:
	
	Map();
	void buildLevel();
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	int mapWidth;
	int mapHeight;
	vector<float> levelvertexData;
	vector<float> leveltexCoordData;
	float TILE_SIZE;
	int SPRITE_COUNT_X;//16
	int SPRITE_COUNT_Y;//8
	unsigned char** levelData;

};