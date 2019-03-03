#pragma once
#include "Player.hpp"

class Pellet : public Player {

public:
	bool active;
	int pelletId;
	std::string name;
	std::vector<int> veggieValues;

	//Pellet(float speed = 0);


	void createPellet(int id, std::string desiredName, std::vector<int> desiredVeggieValues, Texture spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed = 10);
};