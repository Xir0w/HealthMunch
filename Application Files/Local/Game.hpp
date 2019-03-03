#ifndef Game_hpp
#define Game_hpp
#include "MazeData.hpp"
//#include "Ghost.hpp"
#include <sstream>
#include <fstream>

const int STARTING_SPEED = 60;
const int MEDIUM_SPEED = 45;
const int SLOW_SPEED = 30;
const int MINIMUM_SPEED = 20;

const int GHOST_STARTING_SPEED = 60;

const int PELLET_POINT_VALUE = 10;

// GHOST LOGIC
void ghostCollisions(Player &pacman, Ghost &ghost, Audio &eatghost, int &ghostCount, int &score, std::vector<int> &currentScores,
                     int &gamestate, Audio &death, Audio &collision, bool &edible, bool &invincible, Clock &startClock, Clock &invincibilityClock);

void managePlayerState(Player &pacman);
bool checkCollision(Player& object1, Player& object2);
void findNode(Player& player, Node& node);

// SCORE AND LEVELS
void updatePoints(std::ostringstream &ss, int &score, Text &playerScore);
void updatePelletPoints(std::ostringstream &ss, Pellet &pellet, std::vector<Text> &vitaminTexts, std::vector<int> &currentScores, int &veggieToErase, Ghost &blinky, Ghost &inky, Ghost &pinky, Ghost &clyde);
int getHighScore();
void saveHighScore(int score);
void oneUp(int &score, int &lifeScore, int &lifeCount, Audio &life);
void levelUp(int &pelletCount, int &level, float &looppitch, Audio &siren, Player &fruit);
void setDifficulty(Player &blinky, Player &inky, Player &pinky, Player &clyde, float &edibleLimit);

// SOUND
void soundSwitcher(bool &isEdible, int &gamestate, Audio &siren, Audio &scatter);

void toggleMute(bool &isMuted, Player &sound, Audio &chomp1, Audio &chomp2, Audio &scatter, Audio &theme,
                Audio &siren, Audio &eatfruit, Audio &life, Audio &death, Audio &eatghost);

// RESETS
void resetStats(int &lifeCount, int &pelletCount, int &score, int &lifeScore, int &level, float &looppitch,
                Player &fruit, std::ostringstream &ss, Text &playerScore, Clock &startClock);

void resetGame(Player &pacman, Ghost &blinky, Ghost &inky, Ghost &pinky, Ghost &clyde,
               MazeData &maze, int &gamestate);

#endif /* Game_hpp */
