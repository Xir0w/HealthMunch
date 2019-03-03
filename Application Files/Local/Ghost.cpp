#include "Ghost.hpp"

Ghost::Ghost(float speed) : Player(speed) {}

Ghost::Ghost(std::string spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState,
             float speed) : Player(spritesheet, spritesize, numberofStates, framesperState) {}


void Ghost::create(std::string spritesheet, size_t spritesize, size_t numberofStates,
                   size_t framesperState, float speed) {
    Ghost(spritesheet, spritesize, numberofStates, framesperState);
}

int Ghost::findOpposite(int dir) {
    switch (dir) {
        case RIGHT: return LEFT;
        case LEFT: return RIGHT;
        case UP: return DOWN;
        case DOWN: return UP;
    }
}

void Ghost::moveGhost(Time deltaTime, int gstate, float edibleTime, float edibleLimit) {
    if (gstate != PLAYING) {
        pause();
        return;
    }
    
    movement.x = 0.0; movement.y = 0.0;
    
    switch (direction) {
        case RIGHT: movement.x = m_speed;
            break;
        case LEFT: movement.x = -m_speed;
            break;
        case UP: movement.y = -m_speed;
            break;
        case DOWN: movement.y = m_speed;
            break;
    }
    if (isEdible) {
        (edibleTime >= (edibleLimit / 1.5) && edibleTime < edibleLimit) ? setState(BLINK) : setState(SCATTER);
    }
    else {
        switch (direction) {
            case RIGHT: setState(FACERIGHT);
                break;
            case LEFT: setState(FACELEFT);
                break;
            case UP: setState(FACEUP);
                break;
            case DOWN: setState(FACEDOWN);
                break;
        }
    }
    
    move(movement);
    (movement.x > 0.0 || movement.x < 0.0 || movement.y > 0.0 || movement.y < 0.0) ? play() : pause();
}

/*      TODO: IMPLEMENT GHOST AI
         BLINKY LOGIC:
         Follows directly behind PAC-MAN
         Always first out of ghost pen
 
         INKY LOGIC:
         Uses PAC-MAN's position/direction and Blinky's position
         Exits ghost pen after PAC-MAN eats 30 pellets
 
         PINKY LOGIC:
         Ambushes PAC-MAN by positioning himself in his way
         Second out of pen, right on game start
 
         CLYDE LOGIC:
         Follows directly behind PAC-MAN like Blinky but scatters once he's too close
         Exits pen after 1/3 of pellets are eaten
 */

void Ghost::ghostAI() {
    std::vector<int> directions;
    for (int i = 0; i < 4; i++) {
        if (i != findOpposite(direction)) {
            directions.push_back(i);
        }
    }
    queueDirection = directions[rand() % 3];
}
