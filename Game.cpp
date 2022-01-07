/* This is the cpp file of Game. (look at header file for description of Class.) */

#include "Game.h"

//runs main menu and restarts initialization of game
void Game::mainMenu()
{
	fstream fsteps;
	fstream fresults;
	int input;
	while (true)
	{
		_points = 0;
		_currBreadcrumbs = 0;
		_maxBreadcrumbs = 0;
		_lives = 3;
		_ghostCounter = 0;
		_boardCounter = 0;
		_board.clearScreens();

		if (argument.size() > 1)
			if (argument[1] == Silent_GAME && argument[0] == LOAD_GAME) {
				_silent = true;
				_pacman.setSilent();
			}
				
		
		if (argument[0] != LOAD_GAME)
		{
			cout << "(1) Start a new game" << endl;
			cout << "(2) Open Specific Screen" << endl;
			cout << "(8) Present instructions and keys" << endl;
			cout << "(9) EXIT" << endl;
			cin >> input;

		}
		else {
			input = START_GAME;
		}

		string s;

		switch (input) {
		case START_GAME:
			
			_board.getFiles();
			if (_board.getNumberOfScreens() == 0) {
				cout << "Screens not found" << endl;
				break;
			}
			system("cls");

			try {
				_maxBreadcrumbs = _board.readBoardFromFile(_initialPositions, _ghostCounter, _board.getScreen(_boardCounter));
			}
			catch (const char* message) {
				cout << "Error: " << message << endl;
				continue;
			}
			catch(int num)
			{
				if (num == 0) {
					cout << "Error: Fail to open file" << endl;
					continue;
				}
			}

			
			if (!_maxBreadcrumbs)
				break;
			if(argument[0] != LOAD_GAME)
				setDifficulty();
			setNextGame(fsteps, fresults, _board.getScreen(_boardCounter));
			system("cls");
			start(fsteps, fresults);
			if (argument[0] == LOAD_GAME)
				return;
			break;
		case GET_NAME_OF_FILE:
			system("cls");
			cout << "Enter file name: ";
			s = getFileName();
			_maxBreadcrumbs = _board.readBoardFromFile(_initialPositions, _ghostCounter, s);
			if (!_maxBreadcrumbs)
				break;
			setDifficulty();
			setNextGame(fsteps, fresults, s);
			system("cls");
			start(fsteps, fresults);
			break;
		case INSTRUCTIONS:
			system("cls");
			instructions();
			break;
		case EXIT_GAME:
			return;
		}
	}
}

//shows intructions of game
void Game::instructions()
{
	_flushall();

	cout << "Instructions:" << endl;
	cout << "To move Pacman:" << endl;
	cout << "\tLeft = a or A" << endl << "\tRight = d or D" << endl << "\tUp = w or W" << endl << "\tDown = x or X" << endl << "\tStay = s or S" << endl;
	cout << "Press ESC to pause" << endl;
	cout << "Move pacman to eat the breadcrumbs. Avoid the ghosts. Eat all breadcrumbs to win. If you get eaten 3 times you lose.\n" << endl;
	cout << "Press any key to continue...\n" << endl;

	while (true) {
		if (_kbhit()) {
		    _getch();
			system("cls");
			return;
		}
	}
}

//starts the game and runs until game is over.
void Game::start(fstream& fsteps, fstream& fresults)
{
	if(!_silent)
		_board.printBoard(_points, _lives);
	
	if (!_silent)
		_board.updateStats(_points, _lives);
	char key = 'p', temp;
	bool rungame = true;
	bool ghostturn = false;
	bool firstMove = true;
	bool fruitturn = false;
	int i, fileTime;
	if (argument[0] == LOAD_GAME)
		key='s';

	while (rungame) {  //every iteration is a new move for pacman - meaning he moved spots (or stayed in his spot if user pressed 's')
		if(!_silent)
			Sleep(100);
		if (argument[0] != LOAD_GAME)
			userDirection(key);

		if (!_pause) {


			if (_fruit.timetodrop())
			{
				if (argument[0] == LOAD_GAME)
					setFruitFromFile(fsteps);
				else
					dropFruit();
				_fruit.changestate();
				if (argument[0] == SAVE_GAME)
					setFruitFromFile(fsteps);

				if (!_silent) {
					_board.gotoxy(_fruit.getPosition().getX(), _fruit.getPosition().getY());
					cout << _fruit.getSymbol() << endl;
				}
			}

			if (_fruit.timesup())
			{
				_board.gotoxy(_fruit.getPosition().getX(), _fruit.getPosition().getY());
				if (!_silent) {
					if (_board.getSpot(_fruit.getPosition().getX(), _fruit.getPosition().getY()) == symbols[(int)symbols::BREADCRUMB_CHAR])
						cout << symbols[(int)symbols::BREADCRUMB_CHAR] << endl;
					else
						cout << symbols[(int)symbols::EMPTY_SPACE_CHAR] << endl;
				}
				_fruit.setPosition(-1, -1);
				_fruit.changestate();
			}
			if (fruitturn && _fruit.getState()) {
				if (argument[0] == LOAD_GAME) {
					fsteps >> temp;
					_fruit.setDirection(temp);
				}
				else
					_fruit.getRandDirection(_fruit.getPosition(), _pacman.getPosition(), FRUIT, _board);
				_fruit.moveGameObject(_fruit.getPosition(), _fruit.getDirection(), _fruit.getSymbol(), _board);
				_board.gotoxy(0, 0);

				if (argument[0] == SAVE_GAME)
					fsteps << _fruit.getDirection() << " ";
			}

			if (key != 'p') {
				if (argument[0] == LOAD_GAME)
					fsteps >> key;
				_pacman.moveGameObject(_pacman.getPosition(), key, symbols[(int)symbols::PACMAN_CHAR], _board);
				_board.gotoxy(0, 0);
				if (argument[0] == SAVE_GAME)
					fsteps << key << " ";
				++_gameTime;
				if (_board.getSpot(_pacman.getPosition().getX(), _pacman.getPosition().getY()) == symbols[(int)symbols::BREADCRUMB_CHAR]) {
					_board.setSpot(_pacman.getPosition().getX(), _pacman.getPosition().getY(), symbols[(int)symbols::EMPTY_SPACE_CHAR]);
					_points++;
					_currBreadcrumbs++;
				}
				firstMove = false;
			}

			if (!firstMove)
				_fruit.updatetimer();

			if (ghostturn && !firstMove) {
				for (i = 0; i < _ghostCounter; ++i) {
					if (argument[0] == LOAD_GAME) {
						fsteps >> temp;
						_ghosts[i].setDirection(temp);
					}
					else
						_ghosts[i].getRandDirection(_ghosts[i].getPosition(), _pacman.getPosition(), _difficulty, _board);
					_ghosts[i].moveGameObject(_ghosts[i].getPosition(), _ghosts[i].getDirection(), symbols[(int)symbols::GHOST_CHAR], _board);
					_board.gotoxy(0, 0);

					if (argument[0] == SAVE_GAME) {
						fsteps.flush();
						fsteps << _ghosts[i].getDirection() << " ";
					}

				}
				fruitturn = !fruitturn;
			}


			if (!firstMove)
				ghostturn = !ghostturn;
		}
		if (comparePosition(_fruit.getPosition(), _pacman.getPosition()))
		{
			_points += _fruit.getValue();
			_fruit.setPosition(-1, -1);
			_fruit.resettimer();
			_fruit.changestate();
		}
		for(i = 0; i < _ghostCounter; ++i)
			if (comparePosition(_fruit.getPosition(), _ghosts[i].getPosition()))
			{
				_fruit.setPosition(-1, -1);
				_fruit.changestate();
				_fruit.resettimer();
			}
		for (i = 0; i < _ghostCounter; ++i)
			if (comparePosition(_pacman.getPosition(), _ghosts[i].getPosition())) {
				_lives--;
				if (argument[0] == SAVE_GAME) { 
					fresults << DEAD << " ";
					fresults << _gameTime << " ";
				}
				if (_silent) {
					fresults >> temp;
					fresults >> fileTime;
					if (_gameTime != fileTime || temp != DEAD) {
						cout << "Test Fail" << endl;
						return;
					}
				}
				
				if (_fruit.getState()) {
					_fruit.changestate();
				}
				_fruit.resettimer();
				if (!_silent)
					_board.printBoard(_points, _lives);
				setInitialPositions();
				firstMove = true;
				key = 'p';
				if (argument[0] == LOAD_GAME)
					key = 's';
			}

		if (_lives == 0) {
			fsteps.flush();
			fsteps.close();
			fresults.flush();
			fresults.close();
			rungame = false;
			_lives = 3;
			system("cls");
			if (_silent) {
				cout << "Test Passed" << endl;
				return;
			}
			else
				cout << "--- Game Over ---\n" << endl;
			while (true)
			{
				if (_kbhit()) {
					_getch();
					system("cls");
					return;
				}
			}
		}

		if (_currBreadcrumbs == _maxBreadcrumbs) {
			_boardCounter++;
			system("cls");
			if (argument[0] == SAVE_GAME) {
				fresults << WIN << " ";
				fresults << _gameTime << " ";
			}
			if (_silent) {
				fresults >> temp;
				fresults >> fileTime;
				if (_gameTime != fileTime || temp != WIN) {
					cout << "Test Fail" << endl;
					fsteps.close();
					fresults.close();
					return;
				}
			}

			if (_boardCounter == _board.getNumberOfScreens()) {
				rungame = false;

				if (_silent) {
					cout << "Test Passed" << endl;
					fsteps.close();
					fresults.close();
					return;
				}
				
				cout << "--- You Win! ---\n" << endl;
				fsteps.close();
				fresults.close();

				while (true)
				{
					if (_kbhit()) {
						_getch();
						system("cls");
						return;
					}
				}
			}
			else {
				_ghostCounter = 0;
				_currBreadcrumbs = 0;
				_gameTime = 0;
				firstMove = true;
				key = 'p';
				if (argument[0] == LOAD_GAME)
					key = 's';
				_maxBreadcrumbs = _board.readBoardFromFile(_initialPositions, _ghostCounter, _board.getScreen(_boardCounter));

				_fruit.resetTimer();
				setNextGame(fsteps, fresults, _board.getScreen(_boardCounter));
				_board.printBoard(_points, _lives);

			}
		}
		if (!_silent)
			_board.updateStats(_points, _lives);
	    fsteps.flush();
	}
	
}

//gets direction from user and checks if valid
void Game::userDirection(char& key)
{
	char tempkey = 0;
	if (_kbhit()) //If the user hit a key
		tempkey = _getch();
	tempkey = tolower(tempkey);
	if (validUserInput(tempkey)) {
		if (tempkey == ESC)
			_pause = !_pause;
		else {
			if (!_pause)
				key = tempkey;
		}
	}
}

//checks that users input is valid
bool Game::validUserInput(char key)
{
	return (key == ESC || key == directions[(int)directions::STAY] || key == directions[(int)directions::LEFT] || key == directions[(int)directions::RIGHT] || key == directions[(int)directions::UP] || key == directions[(int)directions::DOWN]);
}

//comapres between 2 positions - if equal returns true else false
bool Game::comparePosition(Position& p1, Position& p2)
{
	if (p1.getX() == p2.getX() && p1.getY() == p2.getY())
		return true;
	return false;
}

//drops fruit on board
void Game::dropFruit()
{
	_fruit.setValue(5 + rand() % 5);
	_fruit.setSymbol(_fruit.getValue() + '0');

	Position temp; 

	do {
		temp.setX((rand() % (_board.getHeight() - _board.getTop() - 1)) + _board.getTop() + 1);
		temp.setY((rand() % _board.getWidth() - 2) + 1);
	} while (invalidFruitPosition(temp));
	if(!_silent)
		_board.gotoxy(temp.getX(), temp.getY());

	_fruit.setPosition(temp.getX(), temp.getY());


}

//checks if position of fruit is invalid
bool Game::invalidFruitPosition(Position p)
{
	bool res = (comparePosition(p, _pacman.getPosition()) || _board.getSpot(p.getX(), p.getY()) == symbols[(int)symbols::WALL_CHAR] || _board.getSpot(p.getX(), p.getY()) == symbols[(int)symbols::EMPTY_SPACE_CHAR]);

	if (!res)
		for (int i = 0; res = false, i < _ghostCounter; ++i) {
			res = comparePosition(p, _ghosts[i].getPosition());
		}
	return res;
}

//sets the initial position of all gameObjects
void Game::setInitialPositions()
{
	_pacman.setPosition(_initialPositions[0].getX(), _initialPositions[0].getY());
	for (int i = 1; i <= _ghostCounter; ++i)
		_ghosts[i - 1].setPosition(_initialPositions[i].getX(), _initialPositions[i].getY());
}

//sets difficulty
void Game::setDifficulty()
{
	cout << "Choose Difficulty:" << endl;
	cout << "(a) Best" << endl;
	cout << "(b) Good" << endl;
	cout << "(c) Novice" << endl;

	char temp;
	cin >> temp;

	_difficulty = tolower(temp);
}

//sets timers of all ghosts
void Game::setTimers()
{
	for (int i = 0; i < _ghostCounter; ++i)
	{
		_ghosts[i].setTimer(_difficulty);
	}
}

//sets up next game
void Game::setNextGame(fstream& fsteps, fstream& fresults, string fileName)
{
	
	setInitialPositions();
	setTimers();

	if (argument[0] == LOAD_GAME || argument[0] == SAVE_GAME) {
		try
		{
			setFiles(fsteps, fresults, fileName);
		}
		catch (const char* message)
		{
			cout << message << endl;
			return;
		}
	}
}

//gets specific file from user
string Game::getFileName()
{
	cin.get();
	string c;
		getline(cin, c);
		_board.addScreen(c);
	return c;
}

//gets argument from console
void Game::getArgument(char* in)
{
	if (argument[0] =="")
		argument[0] = in;
	else
	argument.push_back(in);
}

//sets and opens steps and results file according to current screen
void Game::setFiles(fstream& steps, fstream& results, string fileName)
{
	string s1 = fileName;

	if (steps.is_open()) 
		steps.close();
	if (results.is_open())
		results.close();

	s1.replace(s1.find("screen"), 9, "steps.txt");
	if (argument[0] == LOAD_GAME){
		steps.open(s1, ios::in);
	if (!steps.is_open())
		throw "File not found";
	}
	else {
	steps.open(s1, ios::out);
	if (!steps.is_open())
		throw "Error creating file";
	}
		

	s1.replace(s1.find("steps.txt"), 11, "results.txt");
	if (argument[0] == LOAD_GAME) {
		results.open(s1, ios::in);
		if (!results.is_open())
			throw "File not found";
	}
	else {
		results.open(s1, ios::out);
		if (!results.is_open())
			throw "Error creating file";
	}
}

//sets fruits value and position from file during load mode
void Game::setFruitFromFile(fstream& steps)
{
	int temp;
	if (argument[0] == LOAD_GAME) {
		steps >> temp;
		_fruit.setValue(temp);
		_fruit.setSymbol(temp + '0');
		steps >> temp;
		_fruit.getPosition().setX(temp);
		steps >> temp;
		_fruit.getPosition().setY(temp);
	}
	else if (argument[0] == SAVE_GAME) {
		steps << _fruit.getValue() << " ";
		steps << _fruit.getPosition().getX() << " ";
		steps << _fruit.getPosition().getY() << " ";
	}
}


