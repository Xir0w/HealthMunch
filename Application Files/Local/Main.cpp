#include "Game.hpp"

//when the bar completes just keep it at 200 and DONT take the rest off the board im thinking...

int main() {

    RenderWindow window(VideoMode(WinX, WinY), "PAC-MAN 2017", Style::Default);
    View view(Vector2f(WinX / 2, WinY / 2), Vector2f(WinX, WinY));
    window.setKeyRepeatEnabled(false);

	bool invincible = false;
    Clock deltaClock, edibleClock, fruitClock, startClock, invincibilityClock;
    Time deltaTime, startTime, deathTime;
    float edibleTime = 0.0, fruitTime = 0.0;

    MazeData maze;
	int ghostCount = 0, pelletCount = 0, lifeCount = 0, lifeScore = 10000, veggieToErase = -1;
    int gamestate = STARTING, level = 1, score = 0, hiscore = getHighScore();
    bool atLeastOneEdible = false, showFruit = false;
    float looppitch = 1.00, edibleLimit = 10.0;
    std::ostringstream ss;

	//Font font; font.loadFromFile(resourcePath() + "Graphics/font.ttf");
	Font font; font.loadFromFile(resourcePath() + "Graphics/breakfastDEMO.otf");
	//remember to take out playerScore
    Text playerScore("", font), playerHiScore("", font), ready("HUNGRY!", font, 40),
    gameover("GAME   OVER", font, 53), hiScoreText("HI SCORE", font),
		recommendedVitaminAText("Daily Vitamin A: 0.9mg", font, 25),
		recommendedVitaminCText("Daily Vitamin C: 90mg", font, 25),
		recommendedVitaminKText("Daily Vitamin K: 0.12mg", font, 25),
		recommendedCalciumText("Daily Calcium: 1000mg", font, 25),
		carrotInfo("CARROT\n+++Vitamin A\n++Calcium", font, 22),
		orangeInfo("ORANGE\n+++Vitamin C\n++Calcium", font, 22),
		pepperInfo("CHILI PEPPER\n+++Vitamin K\n+Vitamin A\n+Vitamin C", font, 22),
		broccoliInfo("BROCCOLI\n+++Calcium\n++Vitamin K", font, 22),
		appleInfo("APPLE\nAll-Around fruit", font, 22),
		currentScoreText("Munched: 0", font, 25);

	std::vector<int> currentScores;
	for (int i = 0; i < NUM_NUTRIENTS; i++)
	{
		currentScores.push_back(0);
	}

	//The scores in text form.
	std::vector<Text> currentScoreTexts;
	for (int i = 0; i < NUM_NUTRIENTS; i++)
	{
		currentScoreTexts.push_back(currentScoreText);
	}

	//playerScore.setPosition(Vector2f(150, 40));
	//hiScoreText.setPosition(Vector2f(512, 40));
	//playerHiScore.setPosition(Vector2f(675, 40));
	//hiScoreText.setPosition(Vector2f(900, 900));
	playerScore.setPosition(Vector2f(40, 300));
	//playerHiScore.setPosition(Vector2f(675, 40));
	ready.setPosition(Vector2f(460, 550));
    gameover.setPosition(Vector2f(405, 540));

	for (int i = 0; i < NUM_NUTRIENTS; i++)
	{
		currentScoreTexts[i].setFillColor(Color(255, 255, 255)); // WHITE
	}

	//Vitamin A
	recommendedVitaminAText.setFillColor(Color(255, 255, 255));
	recommendedVitaminAText.setPosition(Vector2f(40, 375));
	currentScoreTexts[0].setPosition(Vector2f(90, 410));

	//Vitamin C
	recommendedVitaminCText.setFillColor(Color(255, 255, 255));
	recommendedVitaminCText.setPosition(Vector2f(40, 545));
	currentScoreTexts[1].setPosition(Vector2f(90, 580));

	//Vitamin K
	recommendedVitaminKText.setFillColor(Color(255, 255, 255));
	recommendedVitaminKText.setPosition(Vector2f(815, 375));
	currentScoreTexts[2].setPosition(Vector2f(815, 410));

	//Calcium
	recommendedCalciumText.setFillColor(Color(255, 255, 255));
	recommendedCalciumText.setPosition(Vector2f(815, 545));
	currentScoreTexts[3].setPosition(Vector2f(815, 580));

	//Carrot Info
	carrotInfo.setFillColor(Color(255, 255, 255));
	carrotInfo.setPosition(Vector2f(10, 140));

	//Orange Info
	orangeInfo.setFillColor(Color(255, 255, 255));
	orangeInfo.setPosition(Vector2f(10, 735));

	//Pepper Info
	pepperInfo.setFillColor(Color(255, 255, 255));
	pepperInfo.setPosition(Vector2f(925, 140));

	//Broccoli Info
	broccoliInfo.setFillColor(Color(255, 255, 255));
	broccoliInfo.setPosition(Vector2f(925, 735));

	//Apple Info
	appleInfo.setFillColor(Color(255, 255, 255));
	appleInfo.setPosition(Vector2f(500, 500));

    hiScoreText.setFillColor(Color(208, 62, 25));            // RED
    playerHiScore.setFillColor(Color(255, 255, 255));        // WHITE
    playerScore.setFillColor(Color(255, 255, 255));          // WHITE
    ready.setFillColor(Color(255, 255, 0));                  // YELLOW
    gameover.setFillColor(Color(208, 62, 25));               // RED

	updatePoints(ss, score, playerScore);
	//updatePoints(ss, score, subScores, playerScores);
	updatePoints(ss, hiscore, playerHiScore);

	//Characters
    float pacmanSpeed = STARTING_SPEED, ghostSpeed = GHOST_STARTING_SPEED;
    Player pacman("char-pacman.png", 16, 2, 18); pacman.setFrameTime(seconds(0.025)); pacman.pause();
    Ghost blinky("char-blinky.png", 16, 6, 2); blinky.setFrameTime(seconds(0.25));
    Ghost inky("char-inky.png", 16, 6, 2); inky.setFrameTime(seconds(0.25));
    Ghost pinky("char-pinky.png", 16, 6, 2); pinky.setFrameTime(seconds(0.25));
    Ghost clyde("char-clyde.png", 16, 6, 2); clyde.setFrameTime(seconds(0.25));
    
    float scale = 2.5;
    pacman.setScale(scale, scale);
    blinky.setScale(scale, scale);
    inky.setScale(scale, scale);
    pinky.setScale(scale, scale);
    clyde.setScale(scale, scale);
    
    pacman.setPlayerSpeed(pacmanSpeed);
    blinky.setPlayerSpeed(ghostSpeed);
    inky.setPlayerSpeed(ghostSpeed);
    pinky.setPlayerSpeed(ghostSpeed);
    clyde.setPlayerSpeed(ghostSpeed);

    resetGame(pacman, blinky, inky, pinky, clyde, maze, gamestate);

    //Sound
    bool isMuted = false;
	Audio chomp1("chomp1.wav"), chomp2("chomp2.wav"), scatter("scatter.wav"), theme("theme.wav"), collisionSound("eatfruit.wav");
	Audio siren("siren.wav"), eatfruit("eatfruit.wav"), life("life.wav"), death("death.wav"), eatghost("eatghost.wav");
    scatter.setLoop(true); siren.setLoop(true); theme.play();
    // END SOUNDS

    /*** FRUIT ***/
    Player fruit("fruit-all.png", 16, 6, 1);
    /*fruit.setPosition(Vector2f(512, 576));
    fruit.setScale(scale, scale);

    RectangleShape fruitSprite[6];    // FRUIT LIST
    for (int i = 0; i < 6; i++) {
        fruitSprite[i].setSize(Vector2f(spriteSize, spriteSize));
        fruitSprite[i].setPosition(Vector2f(832 - ((spriteSize * 1.25) * i), 970));
    }

    Texture cherry, strawberry, orange, apple, watermelon, key;
    cherry.loadFromFile(resourcePath() + "Graphics/fruit-cherry.png");
    strawberry.loadFromFile(resourcePath() + "Graphics/fruit-strawberry.png");
    orange.loadFromFile(resourcePath() + "Graphics/fruit-orange.png");
    apple.loadFromFile(resourcePath() + "Graphics/fruit-apple.png");
    watermelon.loadFromFile(resourcePath() + "Graphics/fruit-watermelon.png");
    key.loadFromFile(resourcePath() + "Graphics/fruit-key.png");

    fruitSprite[0].setTexture(&cherry);
    fruitSprite[1].setTexture(&strawberry);
    fruitSprite[2].setTexture(&orange);
    fruitSprite[3].setTexture(&apple);
    fruitSprite[4].setTexture(&watermelon);
    fruitSprite[5].setTexture(&key);*/
    // END FRUIT

    RectangleShape lives[7];
    Texture livesTexture;
    livesTexture.loadFromFile(resourcePath() + "Graphics/maze-lives.png");
    for (int i = 0; i < 7; i++) {
        lives[i].setSize(Vector2f(spriteSize, spriteSize));
        lives[i].setTexture(&livesTexture);
        lives[i].setPosition(Vector2f(150 + ((spriteSize * 1.25) * i), 970));
    }
    
    Player sound("sound.png", 80, 2, 1);
    sound.setPosition(Vector2f(860, 55));
    sound.setScale(Vector2f(0.4, 0.4));

    /*** PAUSE STATE ***/
    Texture pauseBG, selectArrow;
    pauseBG.loadFromFile(resourcePath() + "Graphics/menu-bg.png");
    selectArrow.loadFromFile(resourcePath() + "Graphics/menu-select.png");

    RectangleShape pauseOpacity, pauseMenu, select;
    Vector2f selectPos(380, 365);

    pauseOpacity.setSize(Vector2f(WinX, WinY));
    pauseMenu.setSize(Vector2f(600, 750));
    select.setSize(Vector2f(32, 32));

    pauseOpacity.setPosition(0, 0);
    pauseMenu.setPosition(Vector2f(512, 512));
    pauseMenu.setOrigin(pauseMenu.getSize().x / 2.0, pauseMenu.getSize().y / 2.0);
    select.setPosition(selectPos);

    pauseOpacity.setFillColor(Color(0, 0, 0, 100));
    pauseMenu.setTexture(&pauseBG);
    select.setTexture(&selectArrow);
    
    size_t pauseSize = 50;
    Text paused("PAUSED", font, pauseSize), continueText("CONTINUE", font, pauseSize),
    restartText("RESTART", font, pauseSize), quitText("QUIT", font, pauseSize),
    controlText("CONTROLS", font, pauseSize), WASDText("WASD KEYS >", font, pauseSize-5);

    paused.setFillColor(Color(70, 191, 238));           // CYAN
    continueText.setFillColor(Color(255, 255, 255));    // WHITE
    restartText.setFillColor(Color(255, 255, 255));     // WHITE
    quitText.setFillColor(Color(255, 255, 255));        // WHITE
    controlText.setFillColor(Color(255, 255, 255));     // WHITE
    WASDText.setFillColor(Color(255, 255, 255));        // WHITE

    paused.setPosition(Vector2f(435, 250));
    continueText.setPosition(Vector2f(435, 350));
    restartText.setPosition(Vector2f(435, 400));
    quitText.setPosition(Vector2f(435, 450));
    controlText.setPosition(Vector2f(435, 550));
    WASDText.setPosition(Vector2f(435, 600));
    // END PAUSE STATE

    /******************** MAIN GAME LOOP ********************/
    while (window.isOpen()) {
        deltaTime = deltaClock.restart();
        Event checkEvent;

        while (window.pollEvent(checkEvent)) {
            // WINDOW INTERACTION
            if (checkEvent.type == Event::Closed) { //Window is closed
                window.close();
            }
            else if (checkEvent.type == Event::Resized) { //Window is resized
                ResizeView(window, view);
                if (gamestate == PLAYING) {
                    gamestate = PAUSED;
                    soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                }
            }
            else if (checkEvent.type == Event::LostFocus) { //Window becomes inactive
                if (gamestate == PLAYING) {
                    gamestate = PAUSED;
                    soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                }
            }
            else if (checkEvent.type == Event::KeyPressed && checkEvent.key.code == Keyboard::M) {
                toggleMute(isMuted, sound, chomp1, chomp2, scatter, theme, siren, eatfruit, life, death, eatghost);
            }
            

            // GAME INTERACTION
            if (gamestate == PLAYING) { //Pause
                if (checkEvent.type == Event::KeyPressed && checkEvent.key.code == Keyboard::Space) {
                    gamestate = PAUSED;
                    soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                }
            }
            else if (gamestate == PAUSED) { //Pause Menu
                if (checkEvent.type == Event::KeyPressed) {
                    // CURSOR MOVEMENT
                    if (checkEvent.key.code == Keyboard::Down) {
                        if (selectPos.y + 50 == 515) { //Skip blank space
                            selectPos.y = 565;
                        }
                        else if (selectPos.y + 50 <= 615) {
                            selectPos.y += 50;
                            select.setPosition(selectPos);
                        }
                    }
                    else if (checkEvent.key.code == Keyboard::Up) {
                        if (selectPos.y - 50 == 565) { //Skip blank space
                            selectPos.y = 515;
                        }
                        if (selectPos.y - 50 >= 365) {
                            selectPos.y -= 50;
                            select.setPosition(selectPos);
                        }
                    }
                    else if (checkEvent.key.code == Keyboard::Right) {
                        if (selectPos.y == 615) {
                            pacman.WASDkeys = false;
                            WASDText.setString("< ARROW KEYS");
                        }
                    }
                    else if (checkEvent.key.code == Keyboard::Left) {
                        if (selectPos.y == 615) {
                            pacman.WASDkeys = true;
                            WASDText.setString("WASD KEYS >");
                        }
                    }
                }
                // PERFORM SELECTED ACTION
                if (selectPos.y == 465 && checkEvent.key.code == Keyboard::Return) { //Quit
                    window.close();
                }
                else if (selectPos.y == 415 && checkEvent.key.code == Keyboard::Return) { //Restart
                    resetGame(pacman, blinky, inky, pinky, clyde, maze, gamestate);
                    resetStats(lifeCount, pelletCount, score, lifeScore, level, looppitch, fruit, ss, playerScore, startClock);

                    theme.play();
                    eatfruit.play();
                    gamestate = STARTING;
                }
                else if (selectPos.y == 365 && checkEvent.key.code == Keyboard::Return) { //Continue
                    gamestate = PLAYING;
                    eatfruit.play();
                    soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                }
            }
            else if (gamestate == LOSER) { //Space restarts
                if (checkEvent.type == Event::KeyPressed && checkEvent.key.code == Keyboard::Space) {
                    gamestate = STARTING;
                    resetGame(pacman, blinky, inky, pinky, clyde, maze, gamestate);
                    resetStats(lifeCount, pelletCount, score, lifeScore, level, looppitch, fruit, ss, playerScore, startClock);
                    eatfruit.play();
                    theme.play();
                }
            }
        }

        //Animations
        if (gamestate == STARTING) {
            startTime = startClock.getElapsedTime();
            pacman.manageState();
			pacman.setPlayerSpeed(STARTING_SPEED);
            if (startTime.asSeconds() >= 4.2) {
                gamestate = PLAYING;
                pacman.setState(ALIVE);
                pacman.manageState();
                soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                showFruit = false;
            }
        }
        else if (gamestate == DYING) {
            float elapsedTime = startClock.getElapsedTime().asSeconds();
            if (elapsedTime >= 1.3) {
                resetGame(pacman, blinky, inky, pinky, clyde, maze, gamestate);
                lifeCount--;
                lifeCount < 0 ? gamestate = LOSER : gamestate = STARTING;
                if (gamestate == LOSER && score >= hiscore) {
                    saveHighScore(hiscore);
                    playerHiScore.setFillColor(Color(255, 255, 255));
                }
            }
        }
        else if (gamestate == WINNER) {
            float elapsedTime = startClock.getElapsedTime().asSeconds();
            if (elapsedTime >= 3.0) {
                setDifficulty(blinky, inky, pinky, clyde, edibleLimit);
                soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                levelUp(pelletCount, level, looppitch, siren, fruit);
                resetGame(pacman, blinky, inky, pinky, clyde, maze, gamestate);
                startClock.restart().asSeconds();
                gamestate = STARTING;
            }
        }
        else if (gamestate == PLAYING) {   //Game loop

			gamestate = WINNER;
			for (int i = 0; i < NUM_NUTRIENTS; i++)
			{
				if (currentScores[i] < DAILY_VALUES[i])
					gamestate = PLAYING;
			}

			if (gamestate == WINNER)
			{
				startClock.restart().asSeconds();
				for (int i = 0; i < NUM_NUTRIENTS; i++)
				{
					currentScores[i] = 0;
					currentScoreTexts[i].setFillColor(Color(255, 255, 255)); // WHITE
					life.play();
				}
			}

			if (pelletCount >= 244) {
				startClock.restart().asSeconds();
				gamestate = WINNER;
				pelletCount = 0;
			}

            
			//Movement
            pacman.setDirection();
            for (int i = 0; i < 65; i++) {
                findNode(blinky, maze.node.get()[i]);
                findNode(inky, maze.node.get()[i]);
                findNode(pinky, maze.node.get()[i]);
                findNode(clyde, maze.node.get()[i]);
                findNode(pacman, maze.node.get()[i]);
            }
            for (int i = 0; i < 7; i++) {
                findNode(blinky, maze.ghostNode.get()[i]);
                findNode(inky, maze.ghostNode.get()[i]);
                findNode(pinky, maze.ghostNode.get()[i]);
                findNode(clyde, maze.ghostNode.get()[i]);
            }
            
            blinky.ghostAI(); inky.ghostAI(); pinky.ghostAI(); clyde.ghostAI();

			if (!invincible) {
				ghostCollisions(pacman, blinky, eatghost, ghostCount, score, currentScores, gamestate, death, collisionSound, atLeastOneEdible, invincible, startClock, invincibilityClock);
				ghostCollisions(pacman, inky, eatghost, ghostCount, score, currentScores, gamestate, death, collisionSound, atLeastOneEdible, invincible, startClock, invincibilityClock);
				ghostCollisions(pacman, pinky, eatghost, ghostCount, score, currentScores, gamestate, death, collisionSound, atLeastOneEdible, invincible, startClock, invincibilityClock);
				ghostCollisions(pacman, clyde, eatghost, ghostCount, score, currentScores, gamestate, death, collisionSound, atLeastOneEdible, invincible, startClock, invincibilityClock);
				soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
			}
			else {
				float elapsedTime = invincibilityClock.getElapsedTime().asSeconds();
				if (elapsedTime >= 3.0)
					invincible = false;
			}

			//updatePoints(ss, score, playerScore);
            
            if (score > hiscore) {
                hiscore = score;
                playerHiScore.setFillColor(Color(180, 180, 180));
                updatePoints(ss, hiscore, playerHiScore);
            }
            
            atLeastOneEdible = (blinky.isEdible || inky.isEdible || pinky.isEdible || clyde.isEdible);
            if (atLeastOneEdible) {
                edibleTime = edibleClock.getElapsedTime().asSeconds();

                if (edibleTime >= edibleLimit) {
                    atLeastOneEdible = false;
                    blinky.isEdible = inky.isEdible = pinky.isEdible = clyde.isEdible = false;
                    soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                    ghostCount = 0;
                }
            }

            //Pellets
            for (int i = 0; i < 240; i++) { //Small pellets
                if (checkCollision(pacman, maze.pellet.get()[i])) {
					Pellet currentPellet = maze.pellet.get()[i];
                    score = score + PELLET_POINT_VALUE;
					//get the pellet type
					updatePoints(ss, score, playerScore);
					updatePelletPoints(ss, currentPellet, currentScoreTexts, currentScores, veggieToErase, blinky, inky, pinky, clyde);

                    oneUp(score, lifeScore, lifeCount, life);

                    pelletCount % 2 == 0 ? chomp1.play() : chomp2.play();

                    pelletCount++;
                    if (pelletCount % 50 == 0 && pelletCount != 0) {
                        looppitch += 0.025; //Increase sound pitch for every 50 pellets
                        siren.setPitch(looppitch);
                    }
					maze.pellet.get()[i].setPosition(Vector2f(0, WinY * 10));
                }
            }

			if (veggieToErase != -1)
			{
				for (int i = 0; i < 240; i++)
				{
					if (maze.pellet.get()[i].pelletId == veggieToErase) {
						maze.pellet.get()[i].setPosition(Vector2f(0, WinY * 10));
						pelletCount++;
					}
				}
				veggieToErase = -1;
			}

            for (int i = 0; i < 4; i++) { // Power Pellets
                if (checkCollision(pacman, maze.powerPellet.get()[i])) {
                    score = score + 50;
                    updatePoints(ss, score, playerScore);
                    oneUp(score, lifeScore, lifeCount, life);
                    pelletCount++;
                    
                    blinky.isEdible = inky.isEdible = pinky.isEdible = clyde.isEdible = true;
                    edibleClock.restart();
                    
                    pelletCount % 2 == 0 ? chomp1.play() : chomp2.play();
                    soundSwitcher(atLeastOneEdible, gamestate, siren, scatter);
                    
                    if (pelletCount % 50 == 0 && pelletCount != 0) {
						looppitch += 0.025; //Increase pitch
                        siren.setPitch(looppitch);
                    }
                    maze.powerPellet.get()[i].setPosition(Vector2f(0, WinY * 10));
                }
            }

            /*** FRUIT LOGIC ***/
            /*startTime = startClock.getElapsedTime();
            if (startTime.asSeconds() > 35.0 && startTime.asSeconds() < 35.1) {
                showFruit = true;
                fruitClock.restart().asSeconds();
            }
            if (showFruit) {
                fruitTime = fruitClock.getElapsedTime().asSeconds();
                if (fruitTime >= 0.0 && fruitTime <= 10.0) {
                    if (checkCollision(pacman, fruit)) {
                        switch (level) {
                            case 1: score = score + 100;
                                break;
                            case 2: score = score + 300;
                                break;
                            case 3: score = score + 500;
                                break;
                            case 4: score = score + 700;
                                break;
                            case 5: score = score + 1000;
                                break;
                            default: score = score + 2000;
                                break;
                        }
                        updatePoints(ss, score, playerScore);
                        eatfruit.play();
                        showFruit = false;
                    }
                }
                if (fruitTime >= 9.99 && fruitTime <= 10.0) {
                    showFruit = false;
                }
            }*/

            //Teleport player to opposite side if out of bounds
            maze.teleportPlayer(pacman);
            maze.teleportPlayer(blinky);
            maze.teleportPlayer(pinky);
            maze.teleportPlayer(inky);
            maze.teleportPlayer(clyde);
            
        }
        
        //Move after touching node
        pacman.movePlayer(deltaTime, gamestate);
        blinky.moveGhost(deltaTime, gamestate, edibleTime, edibleLimit);
        inky.moveGhost(deltaTime, gamestate, edibleTime, edibleLimit);
        pinky.moveGhost(deltaTime, gamestate, edibleTime, edibleLimit);
        clyde.moveGhost(deltaTime, gamestate, edibleTime, edibleLimit);
        
        maze.mazeBG.update(deltaTime);
        fruit.update(deltaTime);
        pacman.update(deltaTime);
        blinky.update(deltaTime);
        inky.update(deltaTime);
        pinky.update(deltaTime);
        clyde.update(deltaTime);

        window.setView(view);
        window.clear();

        //Drawing
        maze.draw(window, deltaTime);
        //window.draw(playerScore);
		for (int i = 0; i < NUM_NUTRIENTS; i++)
		{
			window.draw(currentScoreTexts[i]);
		}
		window.draw(recommendedVitaminAText);
		window.draw(recommendedVitaminCText);
		window.draw(recommendedVitaminKText);
		window.draw(recommendedCalciumText);
		window.draw(carrotInfo);
		window.draw(orangeInfo);
		window.draw(pepperInfo);
		window.draw(broccoliInfo);
		//window.draw(appleInfo);
        //window.draw(playerHiScore);
        //window.draw(hiScoreText);
        window.draw(sound);

        for (int i = 0; i < lifeCount; i++) {
            window.draw(lives[i]);
        }

        /*int fruitlimit = level < 7 ? level : 6;
        for (int i = 0; i < fruitlimit; i++) {
            window.draw(fruitSprite[i]);
        */

        if (gamestate != LOSER) {
            window.draw(pacman);
        }
        if (gamestate != DYING && gamestate != WINNER) {
            window.draw(blinky);
            window.draw(inky);
            window.draw(pinky);
            window.draw(clyde);
        }

        if (gamestate == STARTING) {
			pacman.setPlayerSpeed(STARTING_SPEED);
            window.draw(ready);
            maze.mazeBG.pause();
        }
        /*else if (gamestate == PLAYING) {
            if (showFruit) {
                window.draw(fruit);
            }
        }*/
        else if (gamestate == LOSER) {
            window.draw(gameover);
        }
        else if (gamestate == WINNER) {
            maze.mazeBG.play();
        }
        else if (gamestate == PAUSED) {
            window.draw(pauseOpacity);
            window.draw(pauseMenu);
            window.draw(paused);
            window.draw(continueText);
            window.draw(restartText);
            window.draw(quitText);
            window.draw(controlText);
            window.draw(select);
            window.draw(WASDText);
        }
        window.display();
    }
    return 0;
}
