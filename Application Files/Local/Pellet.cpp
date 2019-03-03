#include <Pellet.hpp>
#include <time.h>

void Pellet::createPellet(int id, std::string desiredName, std::vector<int>desiredVeggieValues, Texture spritesheet, size_t spritesize, size_t numberofStates, size_t framesperState, float speed) {
	//active = true;
	pelletId = id;
	name = desiredName;
	veggieValues = desiredVeggieValues;
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