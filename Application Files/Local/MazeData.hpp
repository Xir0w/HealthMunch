#pragma once
#include "Collision.hpp"

const int NUM_PELLET_TYPES = 5;
const int NUM_NUTRIENTS = 4;

static const std::vector<int> DAILY_VALUES = { 900,
											900,
											1200,
											1000 };

class MazeData {
	//Texture mazeTexture, pelletTexture, powerpelletTexture;
	Texture mazeTexture, powerpelletTexture;
	std::vector<Texture> pelletTextures;

public:
    Player mazeBG;
    
    MazeData();
    std::shared_ptr<Player> powerPellet;
    std::shared_ptr<Pellet> pellet;

    std::shared_ptr<Node> node;
    std::shared_ptr<Node> ghostNode;

	void placePellets();
	void placeNodes();
    void setValidNodeMovements();
    void teleportPlayer(Player &player);
    
    void draw(RenderWindow &window, Time &deltaTime);
};
