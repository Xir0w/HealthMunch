#pragma once
#include "Nodes.hpp"
#include <memory>

/* Directions */
#define NONE       -1
#define LEFT        0
#define RIGHT       1
#define UP          2
#define DOWN        3

/* PAC-MAN States */
#define ALIVE       0
#define DEAD        1

/* Ghost States */
#define FACELEFT    0
#define FACERIGHT   1
#define FACEUP      2
#define FACEDOWN    3
#define SCATTER     4
#define BLINK       5

/* Game States */
#define STARTING    0    // WHEN GAME IS AT "READY" SCREEN
#define PLAYING     1    // WHEN PLAYER IS UNPAUSED
#define DYING       2    // WHEN PACMAN IS DYING
#define PAUSED      3    // WHEN GAME IS PAUSED
#define WINNER      4    // WHEN PLAYER COLLECTS ALL PELLETS
#define LOSER       5    // WHEN LIVES DROP TO ZERO

class Player : public AnimatedSprite {
    
protected:	
	std::shared_ptr<Animation> state;
    Texture objtexture;
    IntRect TextureRect;
    int pstate = 0;
    
public:
    int direction = NONE, queueDirection = NONE, tryDirection = NONE;
    bool WASDkeys = false;
    Vector2f movement;
    float m_speed;
    
    Player(float speed = 10);
    Player(std::string spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed = 10);
    void create(Texture spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed = 10);
    
    IntRect getTextureRect();
    
    void setState(size_t playerstate);
    int getState() { return pstate; }
    void manageState();
    
    void setPlayerSpeed(float speed);
    float getPlayerSpeed() { return m_speed * 120; }
    void setAnimationSpeed(float speed);
    
private:
    bool isCurrentDirectionValid(Node node, int choosedirection);
    int findOpposite(int dir);

public:
    void setDirection();
    void setDirectionAtNode(Node node);
    void movePlayer(Time deltaTime, int gstate);
};


