#ifndef Nodes_hpp
#define Nodes_hpp
#include "AnimatedSprite.hpp"

class Node {
    RectangleShape mazenode;
    
public:
    bool isUpValid, isDownValid, isLeftValid, isRightValid;
    
    Node();
    void setPosition(Vector2f position);
    Vector2f getPosition();
    void setValidDirections(bool up, bool down, bool left, bool right);
    
    void draw(RenderWindow& window);
};

#endif /* Nodes_hpp */
