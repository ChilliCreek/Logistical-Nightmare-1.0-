#include"pch.h"
#include"Unit.h"
#include <SFML/Graphics.hpp>
#include<iostream>
#include<iomanip>
#include<sstream>
using namespace sf;

enum class gameStat{PAUSED,RUNNING,WON};

int main() {

	unit unitGrid[12][5];

	gameStat gameStatus = gameStat::RUNNING;
	//player stats
	int turnNumber = 0, factoryOutput0 = 200, factoryOutput1 = 200, factoryOutputDelta0 = 20, factoryOutputDelta1 = 20;
	int spawnUnitLeft_x = 0, spawnUnitRight_x = 11, spawnUnitLeft_y = 0, spawnUnitRight_y = 4;

	VideoMode vm(1440, 900);
	RenderWindow window(vm, "Logistical Nightmare", Style::Fullscreen);
	//mouse position  
    Vector2i mousePosition;

	//The clock
	Clock clock;
	clock.restart();
	Time dt;

	//Background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/plain.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);

	//Creating and loading the terrain

	//Equipment textures
	Texture textureUnits[5];
	textureUnits[0].loadFromFile("graphics/infantry.png"); 
	textureUnits[1].loadFromFile("graphics/anti_tank.png");textureUnits[2].loadFromFile("graphics/panzer_3_b.png"); 
	textureUnits[3].loadFromFile("graphics/panther.png"); textureUnits[4].loadFromFile("graphics/tiger_2_p.png");

	//Font
	Font font;
	font.loadFromFile("font/KOMIKAP_.ttf");

	//Creating the menu bar
	RectangleShape menuBar;
	menuBar.setFillColor(Color(170, 105, 30));
	menuBar.setSize(Vector2f(1440, 150));
	
	//Turn number text
	Text turnDisplay;
	turnDisplay.setFont(font);
	turnDisplay.setCharacterSize(20);
	turnDisplay.setPosition(Vector2f(1200,0));
	turnDisplay.setString("Turn 1");

	//Menu icons
	std::vector <Sprite> spriteMenu = { Sprite(textureUnits[0]),Sprite(textureUnits[1]) ,Sprite(textureUnits[2]) ,Sprite(textureUnits[3]) 
		,Sprite(textureUnits[4]) };
	for (int i = 0; i < 5; i++) {
		spriteMenu[i].setOrigin(spriteMenu[i].getLocalBounds().width / 2.0f, spriteMenu[i].getLocalBounds().height / 2.0f);
		spriteMenu[i].setScale(-0.2, 0.2);
	}
	spriteMenu[2].scale(-0.4, 0.4);
	spriteMenu[4].scale(-6, 6);

	//unit selection frame_left
	RectangleShape unitFrame_left;
	unitFrame_left.setOutlineThickness(2);
	unitFrame_left.setSize(Vector2f(120, 120));
	unitFrame_left.setOrigin(unitFrame_left.getLocalBounds().width / 2.0f, unitFrame_left.getLocalBounds().height / 2.0f);
	unitFrame_left.setOutlineColor(Color::Blue);
	unitFrame_left.setPosition(Vector2f(60, 210));
	unitFrame_left.setFillColor(Color(255, 255, 255, 0));

	//unit selection frame_right
	RectangleShape unitFrame_right;
	unitFrame_right.setOutlineThickness(2);
	unitFrame_right.setSize(Vector2f(120, 120));
	unitFrame_right.setOrigin(unitFrame_right.getLocalBounds().width / 2.0f, unitFrame_right.getLocalBounds().height / 2.0f);
	unitFrame_right.setOutlineColor(Color::Red);
	unitFrame_right.setPosition(Vector2f(1380, 690));
	unitFrame_right.setFillColor(Color(255, 255, 255, 0));
	 
	//Factory output left
	Text factoryOutput_Left;
	factoryOutput_Left.setFont(font);
	factoryOutput_Left.setPosition(Vector2f(0,150));
	factoryOutput_Left.setString("Factory Output = 200");
	factoryOutput_Left.setCharacterSize(15);

	//Factory output right
	Text factoryOutput_Right;
	factoryOutput_Right.setFont(font);
	factoryOutput_Right.setString("Factory Output = 200");
	factoryOutput_Right.setCharacterSize(15);
	factoryOutput_Right.setOrigin(factoryOutput_Right.getLocalBounds().width, factoryOutput_Right.getLocalBounds().height);
	factoryOutput_Right.setPosition(Vector2f(1440, 750));

	//menu selection frame_left
    RectangleShape frame_left;
    frame_left.setOutlineThickness(3);
    frame_left.setSize(Vector2f(240, 150));
    frame_left.setOrigin(frame_left.getLocalBounds().width / 2.0f, frame_left.getLocalBounds().height / 2.0f);
    frame_left.setOutlineColor(Color::Black);
    frame_left.setPosition(Vector2f(120, 75));
    frame_left.setFillColor(Color(255, 255, 255, 0));

	//menu selection frame_right
	RectangleShape frame_right;
	frame_right.setOutlineThickness(3);
	frame_right.setSize(Vector2f(240, 150));
	frame_right.setOrigin(frame_right.getLocalBounds().width / 2.0f, frame_right.getLocalBounds().height / 2.0f);
	frame_right.setOutlineColor(Color::Black);
	frame_right.setPosition(Vector2f(1320, 825));
	frame_right.setFillColor(Color(255, 255, 255, 0));

	//unit zone tint
	RectangleShape tint;
	tint.setFillColor(Color(255, 0, 0, 64));
	tint.setSize(Vector2f(120, 120));
	tint.setOrigin(tint.getLocalBounds().width / 2.0f, tint.getLocalBounds().height / 2.0f);
	//hp number text
	Text health;
	health.setFont(font);
	health.setCharacterSize(12);

	//unit type selection
	unitType unitSelection_left = unitType::INF, unitSelection_right = unitType::INF;

	//event
	Event event;

	//turn readiness status
	bool turnReady_left = false, turnReady_right = false;

	for (int i = 0; i < 5; i++) {
		unitGrid[5][i].create(0, true);
		unitGrid[6][i].create(0, false);
	}


	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && gameStatus == gameStat::PAUSED) {
				gameStatus = gameStat::RUNNING;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (gameStatus == gameStat::RUNNING) {
			//unit selection
			if (Keyboard::isKeyPressed(Keyboard::Num1)) {
				unitSelection_left = unitType::INF;
				frame_left.setPosition(Vector2f(120, 75));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
				unitSelection_left = unitType::AT;
				frame_left.setPosition(Vector2f(360, 75));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num3)) {
				unitSelection_left = unitType::LT;
				frame_left.setPosition(Vector2f(600, 75));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num4)) {
				unitSelection_left = unitType::MT;
				frame_left.setPosition(Vector2f(840, 75));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num5)) {
				unitSelection_left = unitType::HT;
				frame_left.setPosition(Vector2f(1080, 75));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num6)) {
				unitSelection_right = unitType::HT;
				frame_right.setPosition(Vector2f(360, 825));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num7)) {
				unitSelection_right = unitType::MT;
				frame_right.setPosition(Vector2f(600, 825));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num8)) {
				unitSelection_right = unitType::LT;
				frame_right.setPosition(Vector2f(840, 825));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num9)) {
				unitSelection_right = unitType::AT;
				frame_right.setPosition(Vector2f(1080, 825));
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num0)) {
				unitSelection_right = unitType::INF;
				frame_right.setPosition(Vector2f(1320, 825));
				gameStatus = gameStat::PAUSED;
			}
			//Movement left side
			else if (Keyboard::isKeyPressed(Keyboard::W) && unitFrame_left.getPosition().y > 210) {
				if (Keyboard::isKeyPressed(Keyboard::LShift) && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getMovedOrNot() == false && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getLeftOrNot() == true) {
						if (unitGrid[spawnUnitLeft_x][spawnUnitLeft_y - 1].getLeftOrNot() == true || unitGrid[spawnUnitLeft_x][spawnUnitLeft_y - 1].getType() == unitType::EMPTY)unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].move(unitGrid[spawnUnitLeft_x][spawnUnitLeft_y - 1]);
						else unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].battle(unitGrid[spawnUnitLeft_x][spawnUnitLeft_y - 1]);
					}
				}
				unitFrame_left.setPosition(Vector2f(unitFrame_left.getPosition().x, unitFrame_left.getPosition().y - 120));
				spawnUnitLeft_y--;
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::A) && unitFrame_left.getPosition().x > 60) {
				if (Keyboard::isKeyPressed(Keyboard::LShift) && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getMovedOrNot() == false && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getLeftOrNot() == true) {
						if (unitGrid[spawnUnitLeft_x - 1][spawnUnitLeft_y].getLeftOrNot() == true || unitGrid[spawnUnitLeft_x - 1][spawnUnitLeft_y].getType() == unitType::EMPTY)unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].move(unitGrid[spawnUnitLeft_x - 1][spawnUnitLeft_y]);
						else unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].battle(unitGrid[spawnUnitLeft_x - 1][spawnUnitLeft_y]);
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::LShift)) {
					unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].move(unitGrid[spawnUnitLeft_x - 1][spawnUnitLeft_y]);
				}
				unitFrame_left.setPosition(Vector2f(unitFrame_left.getPosition().x - 120, unitFrame_left.getPosition().y));
				spawnUnitLeft_x--;
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::S) && unitFrame_left.getPosition().y < 690) {
				if (Keyboard::isKeyPressed(Keyboard::LShift) && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getMovedOrNot() == false && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getLeftOrNot() == true) {
						if (unitGrid[spawnUnitLeft_x][spawnUnitLeft_y + 1].getLeftOrNot() == true || unitGrid[spawnUnitLeft_x][spawnUnitLeft_y + 1].getType() == unitType::EMPTY)unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].move(unitGrid[spawnUnitLeft_x][spawnUnitLeft_y + 1]);
						else unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].battle(unitGrid[spawnUnitLeft_x][spawnUnitLeft_y + 1]);
					}
				}
				unitFrame_left.setPosition(Vector2f(unitFrame_left.getPosition().x, unitFrame_left.getPosition().y + 120));
				spawnUnitLeft_y++;
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::D) && unitFrame_left.getPosition().x < 1380) {
				if (Keyboard::isKeyPressed(Keyboard::LShift) && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getMovedOrNot() == false && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getLeftOrNot() == true) {
						if (unitGrid[spawnUnitLeft_x + 1][spawnUnitLeft_y].getLeftOrNot() == true || unitGrid[spawnUnitLeft_x + 1][spawnUnitLeft_y].getType() == unitType::EMPTY)unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].move(unitGrid[spawnUnitLeft_x + 1][spawnUnitLeft_y]);
						else unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].battle(unitGrid[spawnUnitLeft_x + 1][spawnUnitLeft_y]);
					}
				}
				unitFrame_left.setPosition(Vector2f(unitFrame_left.getPosition().x + 120, unitFrame_left.getPosition().y));
				spawnUnitLeft_x++;
				gameStatus = gameStat::PAUSED;
			}
			//Movement right side
			else if (Keyboard::isKeyPressed(Keyboard::Up) && unitFrame_right.getPosition().y > 210) {
				if (Keyboard::isKeyPressed(Keyboard::RShift) && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getMovedOrNot() == false && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitRight_x][spawnUnitRight_y].getLeftOrNot() == false) {
						if (unitGrid[spawnUnitRight_x][spawnUnitRight_y - 1].getLeftOrNot() == false || unitGrid[spawnUnitRight_x][spawnUnitRight_y - 1].getType() == unitType::EMPTY)unitGrid[spawnUnitRight_x][spawnUnitRight_y].move(unitGrid[spawnUnitRight_x][spawnUnitRight_y - 1]);
						else unitGrid[spawnUnitRight_x][spawnUnitRight_y].battle(unitGrid[spawnUnitRight_x][spawnUnitRight_y - 1]);
					}
				}
				unitFrame_right.setPosition(Vector2f(unitFrame_right.getPosition().x, unitFrame_right.getPosition().y - 120));
				spawnUnitRight_y--;
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left) && unitFrame_right.getPosition().x > 60) {
				if (Keyboard::isKeyPressed(Keyboard::RShift) && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getMovedOrNot() == false && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitRight_x][spawnUnitRight_y].getLeftOrNot() == false) {
						if (unitGrid[spawnUnitRight_x - 1][spawnUnitRight_y].getLeftOrNot() == false || unitGrid[spawnUnitRight_x - 1][spawnUnitRight_y].getType() == unitType::EMPTY)unitGrid[spawnUnitRight_x][spawnUnitRight_y].move(unitGrid[spawnUnitRight_x - 1][spawnUnitRight_y]);
						else unitGrid[spawnUnitRight_x][spawnUnitRight_y].battle(unitGrid[spawnUnitRight_x - 1][spawnUnitRight_y]);
					}
				}
				unitFrame_right.setPosition(Vector2f(unitFrame_right.getPosition().x - 120, unitFrame_right.getPosition().y));
				spawnUnitRight_x--;
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down) && unitFrame_right.getPosition().y < 690) {
				if (Keyboard::isKeyPressed(Keyboard::RShift) && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getMovedOrNot() == false && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitRight_x][spawnUnitRight_y].getLeftOrNot() == false) {
						if (unitGrid[spawnUnitRight_x][spawnUnitRight_y + 1].getLeftOrNot() == false || unitGrid[spawnUnitRight_x][spawnUnitRight_y + 1].getType() == unitType::EMPTY)unitGrid[spawnUnitRight_x][spawnUnitRight_y].move(unitGrid[spawnUnitRight_x][spawnUnitRight_y + 1]);
						else unitGrid[spawnUnitRight_x][spawnUnitRight_y].battle(unitGrid[spawnUnitRight_x][spawnUnitRight_y + 1]);
					}
				}
				unitFrame_right.setPosition(Vector2f(unitFrame_right.getPosition().x, unitFrame_right.getPosition().y + 120));
				spawnUnitRight_y++;
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right) && unitFrame_right.getPosition().x < 1380) {
				if (Keyboard::isKeyPressed(Keyboard::RShift) && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getMovedOrNot() == false && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getType() != unitType::EMPTY) {
					if (unitGrid[spawnUnitRight_x][spawnUnitRight_y].getLeftOrNot() == false) {
						if (unitGrid[spawnUnitRight_x + 1][spawnUnitRight_y].getLeftOrNot() == false || unitGrid[spawnUnitRight_x + 1][spawnUnitRight_y].getType() == unitType::EMPTY)unitGrid[spawnUnitRight_x][spawnUnitRight_y].move(unitGrid[spawnUnitRight_x + 1][spawnUnitRight_y]);
						else unitGrid[spawnUnitRight_x][spawnUnitRight_y].battle(unitGrid[spawnUnitRight_x + 1][spawnUnitRight_y]);
					}
				}
				unitFrame_right.setPosition(Vector2f(unitFrame_right.getPosition().x + 120, unitFrame_right.getPosition().y));
				spawnUnitRight_x++;
				gameStatus = gameStat::PAUSED;
			}
			//Unit spawning
			else if (Keyboard::isKeyPressed(Keyboard::Q) && spawnUnitLeft_x == 0 && unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].getType() == unitType::EMPTY) {
				if (unit::typeToNum(unitSelection_left) * 10 + 10 <= factoryOutput0) {
					unitGrid[spawnUnitLeft_x][spawnUnitLeft_y].create(unit::typeToNum(unitSelection_left), true);
					factoryOutput0 -= unit::typeToNum(unitSelection_left) * 10 + 10;
					factoryOutput_Left.setString("Factory Output = " + std::to_string(factoryOutput0));
				}
				gameStatus = gameStat::PAUSED;
            }
			else if (Keyboard::isKeyPressed(Keyboard::Slash) && spawnUnitRight_x == 11 && unitGrid[spawnUnitRight_x][spawnUnitRight_y].getType() == unitType::EMPTY) {
				if (unit::typeToNum(unitSelection_right) * 10 + 10 <= factoryOutput1) {
					unitGrid[spawnUnitRight_x][spawnUnitRight_y].create(unit::typeToNum(unitSelection_right), false);
					factoryOutput1 -= unit::typeToNum(unitSelection_right) * 10 + 10;
					factoryOutput_Right.setString("Factory Output = " + std::to_string(factoryOutput1));
				}
				gameStatus = gameStat::PAUSED;
			}
			//turn ready setting
			else if (Keyboard::isKeyPressed(Keyboard::Z) && turnReady_left == false) {
				turnReady_left = true;
				gameStatus = gameStat::PAUSED;
			}
			else if (Keyboard::isKeyPressed(Keyboard::P) && turnReady_right == false) {
				turnReady_right = true;
				gameStatus = gameStat::PAUSED;
			}
		}

		if (turnReady_left == true && turnReady_right == true) {
			turnNumber++;
			turnDisplay.setString("Turn " + std::to_string(turnNumber));
			factoryOutput0 += factoryOutputDelta0;
			factoryOutput_Left.setString("Factory Output = " + std::to_string(factoryOutput0));
			factoryOutput1 += factoryOutputDelta1;
			factoryOutput_Right.setString("Factory Output = " + std::to_string(factoryOutput1));
			turnReady_left = false; turnReady_right = false;
			for (int i = 0; i < 12; i++) {
				for (int j = 0; j < 5; j++) {
					unitGrid[i][j].setMovedOrNot(false);
				}
			}
		}


		dt = clock.restart();
		window.clear();
		window.draw(spriteBackground);

		menuBar.setPosition(Vector2f(0, 0));
		window.draw(menuBar);
		for (int i = 0; i < 5; i++) {
			spriteMenu[i].setPosition(120 + i * 240, 75);
			spriteMenu[i].scale(-1, 1);
			window.draw(spriteMenu[i]);
		}
		window.draw(factoryOutput_Left);
		window.draw(turnDisplay);
		window.draw(frame_left);
		window.draw(unitFrame_left);

		menuBar.setPosition(Vector2f(0, 745));
		window.draw(menuBar);
		for (int i = 0; i < 5; i++) {
			spriteMenu[i].setPosition(1320 - i * 240, 825);
			spriteMenu[i].scale(-1, 1);
			window.draw(spriteMenu[i]);
		}
		window.draw(factoryOutput_Right);
		window.draw(frame_right);
		window.draw(unitFrame_right);

		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 5; j++) {
				if (unit::typeToNum(unitGrid[i][j].getType()) != -1) {
					spriteMenu[unit::typeToNum(unitGrid[i][j].getType())].scale(0.5, 0.5);
					spriteMenu[unit::typeToNum(unitGrid[i][j].getType())].setPosition(i * 120 + 60, j * 120 + 210);
					window.draw(spriteMenu[unit::typeToNum(unitGrid[i][j].getType())]);
					if (unitGrid[i][j].getLeftOrNot() == true) {
						tint.setFillColor(Color(0, 0, 255, 64));
						tint.setPosition(i * 120 + 60, j * 120 + 210);
						window.draw(tint);
					}
					else {
						tint.setFillColor(Color(255, 0, 0, 64));
						tint.setPosition(i * 120 + 60, j * 120 + 210);
						window.draw(tint);
					}
					std::stringstream ss;
					ss << std::fixed << std::setprecision(1) << unitGrid[i][j].getStrength();
					health.setString(ss.str());
					ss.clear();
					health.setPosition(Vector2f(i * 120 + 45, j * 120 + 220));
					window.draw(health);
					spriteMenu[unit::typeToNum(unitGrid[i][j].getType())].scale(2, 2);
				}
			}
		}
		window.display();
	}
}