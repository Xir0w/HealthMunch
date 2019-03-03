#include "MazeData.hpp"
#include <sstream>
//#include <iostream>
#include <stdlib.h> //For srand
#include <time.h> //To seed srand

#define PELLET_SIZE 200
#define POWER_PELLET_SIZE 8

//DONT CHANGE THIS WITHOUT CHANGING NUM_PELLET_TYPES IN THE HEADER
static const std::vector<std::string> texturePaths = { "Graphics/apple-pellet.png",
											"Graphics/orange-pellet.png",
											"Graphics/broccoli-pellet.png",
											"Graphics/carrot-pellet.png",
											"Graphics/pepper-pellet.png"};

static const std::vector<std::string> pelletNames = {"CARROT",
													"ORANGE",
													"PEPPER",
													"BROCCOLI",
													"APPLE"};

//static const std::vector<int> 

static const std::vector<int> carrotVeggieValues = { 5, 1, 1, 3 };
static const std::vector<int> orangeVeggieValues = { 1, 5, 1, 3 };
static const std::vector<int> pepperVeggieValues = { 2, 2, 5, 1 };
static const std::vector<int> broccoliVeggieValues = { 1, 1, 3, 5 };
static const std::vector<int> appleVeggieValues = { 2, 2, 2, 1 };

static std::vector<std::vector<int>> veggieValues = { carrotVeggieValues,
													orangeVeggieValues,
													pepperVeggieValues,
													broccoliVeggieValues,
													appleVeggieValues };

MazeData::MazeData() {
	//Seed random number generator
	srand(time(NULL));

    // Initialize maze
    mazeTexture.loadFromFile(resourcePath() + "Graphics/maze.png");
    mazeBG.create(mazeTexture, 512, 1, 2);
    mazeBG.setScale(Vector2f(2, 2));
    mazeBG.setFrameTime(seconds(0.5));
    mazeBG.setPosition(Vector2f(WinX/2, WinY/2));

    // Initialize power pellets
    powerpelletTexture.loadFromFile(resourcePath() + "Graphics/maze-ppellet.png");
    powerPellet = std::shared_ptr<Player>(new Player[4], std::default_delete<Player[]>());
    for (int i = 0; i < 4; i++) {
		//powerpelletsize is unchanged from 8 because its fine as is
        powerPellet.get()[i].create(powerpelletTexture, POWER_PELLET_SIZE, 1, 2);
        powerPellet.get()[i].setFrameTime(seconds(0.5));
        powerPellet.get()[i].setOrigin(4, 4);
        powerPellet.get()[i].setScale(3, 3);
        powerPellet.get()[i].play();
    }
    
    // Initialize pellets

	//Load the png files into the Textures array
	int numTextures = texturePaths.size();

	Texture tempTexture;
	for (int i = 0; i < numTextures; i++)
	{
		//std::cout << " I: " << i << " AND ThE OTHER THING: ";
		//std::cout<<resourcePath() + texturePaths[i];
		tempTexture.loadFromFile(resourcePath() + texturePaths[i]);
		pelletTextures.push_back(tempTexture);
	}

	//Randomly assign a texture to each pellet
    pellet = std::shared_ptr<Pellet>(new Pellet[240], std::default_delete<Pellet[]>());
    for (int i = 0; i < 240; i++) {
		//changed create(pelletTexture, 8, 2, 1); to create(pelletTexture, 16, 2, 1);
		int pelletNumber = rand() % numTextures;
		pellet.get()[i].createPellet(pelletNumber, pelletNames[pelletNumber], veggieValues[pelletNumber], pelletTextures[pelletNumber], PELLET_SIZE, 2, 1);
        pellet.get()[i].setOrigin(4, 4);
        pellet.get()[i].play();
    }
    
    // Initialize all nodes
    ghostNode = std::shared_ptr<Node>(new Node[7], std::default_delete<Node[]>());
    node = std::shared_ptr<Node>(new Node[65], std::default_delete<Node[]>());
    
    placePellets(); placeNodes(); setValidNodeMovements();
    mazeBG.pause();
}

void MazeData::placePellets() {
	// Power Pellets
    powerPellet.get()[0].setPosition(Vector2f(157, 179));   // top left
    powerPellet.get()[1].setPosition(Vector2f(157, 746));   // bottom left
    powerPellet.get()[2].setPosition(Vector2f(869, 179));   // top right
    powerPellet.get()[3].setPosition(Vector2f(869, 746));   // bottom right
    
	// Pellets
    // ROW 1 LEFT
    for (int i = 0; i < 12; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * i), 121));
    }
    // ROW 1 RIGHT
    for (int i = 12; i < 24; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i + 2)), 121));
    }
    
    // ROW 2
    for (int i = 24; i < 50; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 24)), 235));
    }
    
    // ROW 3 LEFT 1
    for (int i = 50; i < 56; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 50)), 319));
    }
    // ROW 3 LEFT 2
    for (int i = 56; i < 60; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 48)), 319));
    }
    
    // ROW 3 RIGHT 1
    for (int i = 60; i < 64; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 46)), 319));
    }
    
    // ROW 3 RIGHT 2
    for (int i = 64; i < 70; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 44)), 319));
    }
    
    // ROW 4 LEFT
    for (int i = 70; i < 82; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 70)), 659));
    }
    // ROW 4 RIGHT
    for (int i = 82; i < 94; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 68)), 659));
    }
    
    // ROW 5 LEFT 1
    for (int i = 94; i < 96; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 93)), 746));
    }
    // ROW 5 LEFT 2
    for (int i = 96; i < 103; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 91)), 746));
    }
    // ROW 5 RIGHT 1
    for (int i = 103; i < 110; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 89)), 746));
    }
    // ROW 5 RIGHT 2
    for (int i = 110; i < 112; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 87)), 746));
    }
    
    // ROW 6 LEFT 1
    for (int i = 112; i < 118; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 112)), 830));
    }
    // ROW 6 LEFT 2
    for (int i = 118; i < 122; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 110)), 830));
    }
    // ROW 6 RIGHT 1
    for (int i = 122; i < 126; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 108)), 830));
    }
    // ROW 6 RIGHT 2
    for (int i = 126; i < 132; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 106)), 830));
    }
    // ROW 7
    for (int i = 132; i < 158; i++) {
        pellet.get()[i].setPosition(Vector2f(157 + (28.5 * (i - 132)), 916));
    }
    
    // COLUMN 1
    pellet.get()[158].setPosition(Vector2f(157, 121 + (28.5 * (1))));
    pellet.get()[159].setPosition(Vector2f(157, 121 + (28.5 * (3))));
    pellet.get()[160].setPosition(Vector2f(157, 121 + (28.5 * (5))));
    pellet.get()[161].setPosition(Vector2f(157, 121 + (28.5 * (6))));
    pellet.get()[162].setPosition(Vector2f(157, 121 + (28.5 * (20))));
    pellet.get()[163].setPosition(Vector2f(157, 121 + (28.5 * (21))));
    pellet.get()[164].setPosition(Vector2f(157, 121 + (28.5 * (26))));
    pellet.get()[165].setPosition(Vector2f(157, 121 + (28.5 * (27))));
    
    // COLUMN 2
    pellet.get()[166].setPosition(Vector2f(214, 121 + (28.5 * (23))));
    pellet.get()[167].setPosition(Vector2f(214, 121 + (28.5 * (24))));
    
    // COLUMN 3
    pellet.get()[168].setPosition(Vector2f(299, 121 + (28.5 * (1))));
    pellet.get()[169].setPosition(Vector2f(299, 121 + (28.5 * (2))));
    pellet.get()[170].setPosition(Vector2f(299, 121 + (28.5 * (3))));
    pellet.get()[171].setPosition(Vector2f(299, 121 + (28.5 * (5))));
    pellet.get()[172].setPosition(Vector2f(299, 121 + (28.5 * (6))));
    for (int i = 173; i < 184; i++) {
        pellet.get()[i].setPosition(Vector2f(299, 121 + (28.5 * (i - 165))));
    }
    pellet.get()[184].setPosition(Vector2f(299, 121 + (28.5 * (20))));
    pellet.get()[185].setPosition(Vector2f(299, 121 + (28.5 * (21))));
    pellet.get()[186].setPosition(Vector2f(299, 121 + (28.5 * (23))));
    pellet.get()[187].setPosition(Vector2f(299, 121 + (28.5 * (24))));
    
    // COLUMN 4
    pellet.get()[188].setPosition(Vector2f(385, 121 + (28.5 * (5))));
    pellet.get()[189].setPosition(Vector2f(385, 121 + (28.5 * (6))));
    pellet.get()[190].setPosition(Vector2f(385, 121 + (28.5 * (23))));
    pellet.get()[191].setPosition(Vector2f(385, 121 + (28.5 * (24))));
    
    // COLUMN 5
    pellet.get()[192].setPosition(Vector2f(470, 121 + (28.5 * (1))));
    pellet.get()[193].setPosition(Vector2f(470, 121 + (28.5 * (2))));
    pellet.get()[194].setPosition(Vector2f(470, 121 + (28.5 * (3))));
    pellet.get()[195].setPosition(Vector2f(470, 121 + (28.5 * (20))));
    pellet.get()[196].setPosition(Vector2f(470, 121 + (28.5 * (21))));
    pellet.get()[197].setPosition(Vector2f(470, 121 + (28.5 * (26))));
    pellet.get()[198].setPosition(Vector2f(470, 121 + (28.5 * (27))));
    
    // HALF WAY POINT (ALL VERTICAL COLUMNS CAN BE MIRRORED)
    
    // COLUMN 6
    pellet.get()[199].setPosition(Vector2f(556, 121 + (28.5 * (1))));
    pellet.get()[200].setPosition(Vector2f(556, 121 + (28.5 * (2))));
    pellet.get()[201].setPosition(Vector2f(556, 121 + (28.5 * (3))));
    pellet.get()[202].setPosition(Vector2f(556, 121 + (28.5 * (20))));
    pellet.get()[203].setPosition(Vector2f(556, 121 + (28.5 * (21))));
    pellet.get()[204].setPosition(Vector2f(556, 121 + (28.5 * (26))));
    pellet.get()[205].setPosition(Vector2f(556, 121 + (28.5 * (27))));
    
    // COLUMN 7
    pellet.get()[206].setPosition(Vector2f(641, 121 + (28.5 * (5))));
    pellet.get()[207].setPosition(Vector2f(641, 121 + (28.5 * (6))));
    pellet.get()[208].setPosition(Vector2f(641, 121 + (28.5 * (23))));
    pellet.get()[209].setPosition(Vector2f(641, 121 + (28.5 * (24))));
    
    // COLUMN 8
    pellet.get()[210].setPosition(Vector2f(727, 121 + (28.5 * (1))));
    pellet.get()[211].setPosition(Vector2f(727, 121 + (28.5 * (2))));
    pellet.get()[212].setPosition(Vector2f(727, 121 + (28.5 * (3))));
    pellet.get()[213].setPosition(Vector2f(727, 121 + (28.5 * (5))));
    pellet.get()[214].setPosition(Vector2f(727, 121 + (28.5 * (6))));
    for (int i = 215; i < 226; i++) {
        pellet.get()[i].setPosition(Vector2f(727, 121 + (28.5 * (i - 207))));
    }
    pellet.get()[226].setPosition(Vector2f(727, 121 + (28.5 * (20))));
    pellet.get()[227].setPosition(Vector2f(727, 121 + (28.5 * (21))));
    pellet.get()[228].setPosition(Vector2f(727, 121 + (28.5 * (23))));
    pellet.get()[229].setPosition(Vector2f(727, 121 + (28.5 * (24))));
    
    // COLUMN 9
    pellet.get()[230].setPosition(Vector2f(812, 121 + (28.5 * (23))));
    pellet.get()[231].setPosition(Vector2f(812, 121 + (28.5 * (24))));
    
    // COLUMN 10
    pellet.get()[232].setPosition(Vector2f(869, 121 + (28.5 * (1))));
    pellet.get()[233].setPosition(Vector2f(869, 121 + (28.5 * (3))));
    pellet.get()[234].setPosition(Vector2f(869, 121 + (28.5 * (5))));
    pellet.get()[235].setPosition(Vector2f(869, 121 + (28.5 * (6))));
    pellet.get()[236].setPosition(Vector2f(869, 121 + (28.5 * (20))));
    pellet.get()[237].setPosition(Vector2f(869, 121 + (28.5 * (21))));
    pellet.get()[238].setPosition(Vector2f(869, 121 + (28.5 * (26))));
    pellet.get()[239].setPosition(Vector2f(869, 121 + (28.5 * (27))));
}

void MazeData::placeNodes() {
    ghostNode.get()[0].setPosition(Vector2f(512, 405));
    ghostNode.get()[1].setPosition(Vector2f(463, 475));
	ghostNode.get()[2].setPosition(Vector2f(512, 475));
	ghostNode.get()[3].setPosition(Vector2f(562, 475));
	ghostNode.get()[4].setPosition(Vector2f(463, 501));
	ghostNode.get()[5].setPosition(Vector2f(512, 501));
	ghostNode.get()[6].setPosition(Vector2f(562, 501));

	// ROW 1
	node.get()[0].setPosition(pellet.get()[0].getPosition());
	node.get()[1].setPosition(pellet.get()[5].getPosition());
	node.get()[2].setPosition(pellet.get()[11].getPosition());
	node.get()[3].setPosition(pellet.get()[12].getPosition());
	node.get()[4].setPosition(pellet.get()[18].getPosition());
	node.get()[5].setPosition(pellet.get()[23].getPosition());

	// ROW 2
	node.get()[6].setPosition(pellet.get()[24].getPosition());
	node.get()[7].setPosition(pellet.get()[29].getPosition());
	node.get()[8].setPosition(pellet.get()[32].getPosition());
	node.get()[9].setPosition(pellet.get()[35].getPosition());
	node.get()[10].setPosition(pellet.get()[38].getPosition());
	node.get()[11].setPosition(pellet.get()[41].getPosition());
	node.get()[12].setPosition(pellet.get()[44].getPosition());
	node.get()[13].setPosition(pellet.get()[49].getPosition());

	// ROW 3
	node.get()[14].setPosition(pellet.get()[50].getPosition());
	node.get()[15].setPosition(pellet.get()[55].getPosition());
	node.get()[16].setPosition(pellet.get()[56].getPosition());

	node.get()[17].setPosition(Vector2f(pellet.get()[59].getPosition().x - 1, pellet.get()[59].getPosition().y));
	node.get()[18].setPosition(Vector2f(pellet.get()[60].getPosition().x - 1, pellet.get()[60].getPosition().y));

	node.get()[19].setPosition(pellet.get()[63].getPosition());
	node.get()[20].setPosition(pellet.get()[64].getPosition());
	node.get()[21].setPosition(pellet.get()[69].getPosition());

	// PORTAL ROW
	node.get()[22].setPosition(pellet.get()[178].getPosition());
	node.get()[23].setPosition(Vector2f(pellet.get()[178].getPosition().x + 86, pellet.get()[178].getPosition().y));
	node.get()[24].setPosition(Vector2f(pellet.get()[220].getPosition().x - 86, pellet.get()[220].getPosition().y));
	node.get()[25].setPosition(pellet.get()[220].getPosition());

	// GHOST HOUSE (TOP)
	node.get()[26].setPosition(Vector2f(pellet.get()[175].getPosition().x + 86, pellet.get()[175].getPosition().y - 1));
	node.get()[27].setPosition(Vector2f(pellet.get()[175].getPosition().x + 170, pellet.get()[175].getPosition().y - 1));
	node.get()[28].setPosition(Vector2f(pellet.get()[217].getPosition().x - (86 * 2), pellet.get()[217].getPosition().y - 1));
	node.get()[29].setPosition(Vector2f(pellet.get()[217].getPosition().x - 86, pellet.get()[217].getPosition().y - 1));

	// GHOST HOUSE (BOTTOM)
	node.get()[30].setPosition(Vector2f(pellet.get()[181].getPosition().x + 86, pellet.get()[181].getPosition().y - 2));
	node.get()[31].setPosition(Vector2f(pellet.get()[223].getPosition().x - 86, pellet.get()[223].getPosition().y - 2));

	// ROW 4
	node.get()[32].setPosition(pellet.get()[70].getPosition());
	node.get()[33].setPosition(pellet.get()[75].getPosition());
	node.get()[34].setPosition(pellet.get()[78].getPosition());
	node.get()[35].setPosition(pellet.get()[81].getPosition());
	node.get()[36].setPosition(pellet.get()[82].getPosition());
	node.get()[37].setPosition(pellet.get()[85].getPosition());
	node.get()[38].setPosition(pellet.get()[88].getPosition());
	node.get()[39].setPosition(pellet.get()[93].getPosition());

	// ROW 5
	node.get()[40].setPosition(powerPellet.get()[1].getPosition());
	node.get()[41].setPosition(pellet.get()[95].getPosition());

	node.get()[42].setPosition(pellet.get()[96].getPosition());
	node.get()[43].setPosition(pellet.get()[99].getPosition());
	node.get()[44].setPosition(pellet.get()[102].getPosition());
	node.get()[45].setPosition(Vector2f(512, 746));
	node.get()[46].setPosition(pellet.get()[103].getPosition());
	node.get()[47].setPosition(pellet.get()[106].getPosition());
	node.get()[48].setPosition(pellet.get()[109].getPosition());
	
	node.get()[49].setPosition(pellet.get()[110].getPosition());
	node.get()[50].setPosition(powerPellet.get()[3].getPosition());

	// ROW 6
	node.get()[51].setPosition(pellet.get()[112].getPosition());
	node.get()[52].setPosition(pellet.get()[114].getPosition());
	node.get()[53].setPosition(pellet.get()[117].getPosition());
	node.get()[54].setPosition(pellet.get()[118].getPosition());
	node.get()[55].setPosition(pellet.get()[121].getPosition());
	node.get()[56].setPosition(pellet.get()[122].getPosition());
	node.get()[57].setPosition(pellet.get()[125].getPosition());
	node.get()[58].setPosition(pellet.get()[126].getPosition());
	node.get()[59].setPosition(pellet.get()[129].getPosition());
	node.get()[60].setPosition(pellet.get()[131].getPosition());

	// ROW 7
	node.get()[61].setPosition(pellet.get()[132].getPosition());
	node.get()[62].setPosition(pellet.get()[143].getPosition());
	node.get()[63].setPosition(pellet.get()[146].getPosition());
	node.get()[64].setPosition(pellet.get()[157].getPosition());

}

void MazeData::setValidNodeMovements() {
    // Parameters: up, down, left, right
    ghostNode.get()[0].setValidDirections(false, false, true, true);
    
    ghostNode.get()[1].setValidDirections(false, false, false, true);
    ghostNode.get()[2].setValidDirections(true, false, true, true);
    ghostNode.get()[3].setValidDirections(false, true, true, false);
    
    ghostNode.get()[4].setValidDirections(true, false, false, true);
    ghostNode.get()[5].setValidDirections(true, false, false, false);
    ghostNode.get()[6].setValidDirections(true, false, true, false);
    
    // ROW 1
    node.get()[0].setValidDirections(false, true, false, true);
    node.get()[1].setValidDirections(false, true, true, true);
    node.get()[2].setValidDirections(false, true, true, false);
    node.get()[3].setValidDirections(false, true, false, true);
    node.get()[4].setValidDirections(false, true, true, true);
    node.get()[5].setValidDirections(false, true, true, false);
    
    // ROW 2
    node.get()[6].setValidDirections(true, true, false, true);
    node.get()[7].setValidDirections(true, true, true, true);
    node.get()[8].setValidDirections(false, true, true, true);
    node.get()[9].setValidDirections(true, false, true, true);
    node.get()[10].setValidDirections(true, false, true, true);
    node.get()[11].setValidDirections(false, true, true, true);
    node.get()[12].setValidDirections(true, true, true, true);
    node.get()[13].setValidDirections(true, true, true, false);
    
    // ROW 3
    node.get()[14].setValidDirections(true, false, false, true);
    node.get()[15].setValidDirections(true, true, true, false);
    node.get()[16].setValidDirections(true, false, false, true);
    node.get()[17].setValidDirections(false, true, true, false);
    node.get()[18].setValidDirections(false, true, false, true);
    node.get()[19].setValidDirections(true, false, true, false);
    node.get()[20].setValidDirections(true, true, false, true);
    node.get()[21].setValidDirections(true, false, true, false);
    
    // PORTAL ROW
    node.get()[22].setValidDirections(true, true, true, true);
    node.get()[23].setValidDirections(true, true, true, false);
    node.get()[24].setValidDirections(true, true, false, true);
    node.get()[25].setValidDirections(true, true, true, true);
    
    // GHOST HOUSE (TOP)
    node.get()[26].setValidDirections(false, true, false, true);
    node.get()[27].setValidDirections(true, false, true, true);
    node.get()[28].setValidDirections(true, false, true, true);
    node.get()[29].setValidDirections(false, true, true, false);
    
    // GHOST HOUSE (BOTTOM)
    node.get()[30].setValidDirections(true, true, false, true);
    node.get()[31].setValidDirections(true, true, true, false);
    
    // ROW 4
    node.get()[32].setValidDirections(false, true, false, true);
    node.get()[33].setValidDirections(true, true, true, true);
    node.get()[34].setValidDirections(true, false, true, true);
    node.get()[35].setValidDirections(false, true, true, false);
    node.get()[36].setValidDirections(false, true, false, true);
    node.get()[37].setValidDirections(true, false, true, true);
    node.get()[38].setValidDirections(true, true, true, true);
    node.get()[39].setValidDirections(false, true, true, false);
    
    // ROW 5
    node.get()[40].setValidDirections(true, false, false, true);
    node.get()[41].setValidDirections(false, true, true, false);
    node.get()[42].setValidDirections(true, true, false, true);
    node.get()[43].setValidDirections(false, true, true, true);
    node.get()[44].setValidDirections(true, false, true, true);
    
    node.get()[45].setValidDirections(false, false, true, true); // NODE 45: PACMAN POSITION
    
    node.get()[46].setValidDirections(true, false, true, true);
    node.get()[47].setValidDirections(false, true, true, true);
    node.get()[48].setValidDirections(true, true, true, false);
    node.get()[49].setValidDirections(false, true, false, true);
    node.get()[50].setValidDirections(true, false, true, false);
    
    // ROW 6
    node.get()[51].setValidDirections(false, true, false, true);
    node.get()[52].setValidDirections(true, false, true, true);
    node.get()[53].setValidDirections(true, false, true, false);
    node.get()[54].setValidDirections(true, false, false, true);
    node.get()[55].setValidDirections(false, true, true, false);
    node.get()[56].setValidDirections(false, true, false, true);
    node.get()[57].setValidDirections(true, false, true, false);
    node.get()[58].setValidDirections(true, false, false, true);
    node.get()[59].setValidDirections(true, false, true, true);
    node.get()[60].setValidDirections(false, true, true, false);
    
    // ROW 7
    node.get()[61].setValidDirections(true, false, false, true);
    node.get()[62].setValidDirections(true, false, true, true);
    node.get()[63].setValidDirections(true, false, true, true);
    node.get()[64].setValidDirections(true, false, true, false);
}

void MazeData::teleportPlayer(Player &player) {
    if (player.getPosition().x <= 100) {
        player.setPosition(Vector2f(WinX - 100, player.getPosition().y));
    }
    else if (player.getPosition().x >= WinX - 100) {
        player.setPosition(Vector2f(100, player.getPosition().y));
    }
    else if (player.getPosition().y <= 0) {
        player.setPosition(Vector2f(player.getPosition().x, WinY));
    }
    else if (player.getPosition().y >= WinY) {
        player.setPosition(Vector2f(player.getPosition().x, 0));
    }
}

void MazeData::draw(RenderWindow &window, Time &deltaTime) {
    window.draw(mazeBG);
    
    for (int i = 0; i < 240; i++) {   // SMALL PELLETS
        if (pellet.get()[i].getPosition().y <= WinY) {
            window.draw(pellet.get()[i]);
        }
    }
    
    for (int i = 0; i < 4; i++) {       // POWER PELLETS
        if (powerPellet.get()[i].getPosition().y <= WinY) {
            powerPellet.get()[i].update(deltaTime);
            window.draw(powerPellet.get()[i]);
        }
    }
}

