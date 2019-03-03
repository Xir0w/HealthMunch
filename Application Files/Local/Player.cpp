#include "Player.hpp"
#include <time.h>

Player::Player(float speed) {
    play();
}

Player::Player(std::string spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed) {
    IntRect textrect(0, 0, spritesize, spritesize);
    TextureRect = textrect;
    
    setOrigin(spritesize / 2, spritesize / 2);
    objtexture.loadFromFile(resourcePath() + "Graphics/" + spritesheet);
    state = std::shared_ptr<Animation>(new Animation[numberofStates], std::default_delete<Animation[]>());
    
    for (int i = 0; i < numberofStates; i++) {
        state.get()[i].setSpriteSheet(objtexture);
        
        for (int j = 0; j < framesperState; j++) {
            state.get()[i].addFrame(IntRect(j * spritesize, i * spritesize, spritesize, spritesize));
        }
    }
    play(state.get()[0]);
}

void Player::create(Texture spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed) {
    setOrigin(spritesize / 2, spritesize / 2);
    objtexture = spritesheet;
    state = std::shared_ptr<Animation>(new Animation[numberofStates], std::default_delete<Animation[]>());

    for (int i = 0; i < numberofStates; i++) {
        state.get()[i].setSpriteSheet(objtexture);

        for (int j = 0; j < framesperState; j++) {
            state.get()[i].addFrame(IntRect(j * spritesize, i * spritesize, spritesize, spritesize));
        }
    }
    play(state.get()[0]);
}

IntRect Player::getTextureRect() {
    return TextureRect;
}

void Player::setState(size_t playerstate) {
    pstate = playerstate;
    play(state.get()[playerstate]);
}

void Player::manageState() {
    if (getState() == ALIVE) {
        setFrameTime(seconds(0.01));
        setLooped(true);
    }
    else if (getState() == DEAD) {
        setFrameTime(seconds(0.075));
        setLooped(false);
        setFrame(0);
        direction = NONE;
        setRotation(0);
    }
}

void Player::setPlayerSpeed(float speed) {    
    m_speed = speed / 120;
}

void Player::setAnimationSpeed(float speed) {
    setFrameTime(seconds(speed));
}

bool Player::isCurrentDirectionValid(Node node, int dir) {
    if (dir == UP && node.isUpValid) {
        return true;
    }
    else if (dir == DOWN && node.isDownValid) {
        return true;
    }
    else if (dir == LEFT && node.isLeftValid) {
        return true;
    }
    else if (dir == RIGHT && node.isRightValid) {
        return true;
    }
    return false;
}

int Player::findOpposite(int dir) {
    switch (dir) {
        case RIGHT: return LEFT;
        case LEFT: return RIGHT;
        case UP: return DOWN;
        case DOWN: return UP;
    }
}

void Player::setDirection() {
    if (WASDkeys) {
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            queueDirection = LEFT;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::D)) {
            queueDirection = RIGHT;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::W)) {
            queueDirection = UP;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::S)) {
            queueDirection = DOWN;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
    }
    else {
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            queueDirection = LEFT;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right)) {
            queueDirection = RIGHT;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up)) {
            queueDirection = UP;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down)) {
            queueDirection = DOWN;
            if (queueDirection == findOpposite(direction)) {
                direction = findOpposite(direction);
            }
        }
    }
}

void Player::setDirectionAtNode(Node node) {
    if (queueDirection == UP) {
        if (node.isUpValid) {
            direction = UP;
        }
        else {
            if (!isCurrentDirectionValid(node, direction)) {
                direction = NONE;
            }
        }
    }
    else if (queueDirection == DOWN) {
        if (node.isDownValid) {
            direction = DOWN;
        }
        else {
            if (!isCurrentDirectionValid(node, direction)) {
                direction = NONE;
            }
        }
    }
    else if (queueDirection == LEFT) {
        if (node.isLeftValid) {
            direction = LEFT;
        }
        else {
            if (!isCurrentDirectionValid(node, direction)) {
                direction = NONE;
            }
        }
    }
    else if (queueDirection == RIGHT) {
        if (node.isRightValid) {
            direction = RIGHT;
        }
        else {
            if (!isCurrentDirectionValid(node, direction)) {
                direction = NONE;
            }
        }
    }
}

void Player::movePlayer(Time deltaTime, int gstate) {
    if (gstate != PLAYING) {
        if (getState() == DEAD) {
            play();
            return;
        }
        pause();
        return;
    }
    
    movement.x = 0.0; movement.y = 0.0;

    if (direction == RIGHT) {
        setRotation(0);
        movement.x = m_speed;
    }
    else if (direction == LEFT) {
        setRotation(180);
        movement.x = -m_speed;
    }
    else if (direction == UP) {
        setRotation(270);
        movement.y = -m_speed;
    }
    else if (direction == DOWN) {
        setRotation(90);
        movement.y = m_speed;
    }

    move(movement);
    (movement.x > 0.0 || movement.x < 0.0 || movement.y > 0.0 || movement.y < 0.0) ? play() : pause();
}
