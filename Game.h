/* This Class creates, runs, and ends the game. It has a visual board which shows current points and lives the user has. It has a pacman and 2 ghosts. */

#pragma once
#ifndef __GAME_H
#define __GAME_H

#include "GameObject.h"
#include "Fruit.h"
#include "Board.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> //functions with input from keyboards

#define START_GAME 1
#define GET_NAME_OF_FILE 2
#define INSTRUCTIONS 8
#define EXIT_GAME 9
#define LOAD_GAME "-load"
#define SAVE_GAME "-save"
#define Silent_GAME "-silent"
#define DEAD 'D'
#define WIN 'W'

class Game
{
private:
	int _lives = 3;
	int _points = 0;
	int _currBreadcrumbs = 0;
	int _maxBreadcrumbs = 0;
	GameObject _pacman;
	GameObject _ghosts[4];

	Fruit _fruit;
	bool _pause = false;
	Board _board;
	Position _initialPositions[6];
	int _ghostCounter = 0;
	char _difficulty;
	int _boardCounter = 0;
	vector<string> argument{""};
	bool _silent = false;
	int _gameTime = 0;

public:
	void mainMenu();
	void instructions();
	void start(fstream& fsteps, fstream& fresults);
	void userDirection(char& key);
	bool validUserInput(char key);
	bool comparePosition(Position& p1, Position& p2);
	void dropFruit();
	bool invalidFruitPosition(Position& p);
	void setInitialPositions();
	void setDifficulty();
	void setTimers();
	void setNextGame(fstream& fsteps, fstream& fresults, string fileName);
	const string getFileName();
	void getArgument(char* in);
	
	void setFiles(fstream& steps, fstream& results, string fileName);
	void setFruitFromFile(fstream& steps);
};

#endif //!__GAME_H

