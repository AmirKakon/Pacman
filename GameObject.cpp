/* This is the cpp file of GameObject. (look at header file for description of Class.) */


#include "GameObject.h"

bool GameObject::_silent = false;

const char GameObject::getDirection()
{
    return this->_direction;
}

void GameObject::setDirection(const char direction)
{
    this->_direction = direction;
}

void GameObject::setPosition(const int x, const  int y)
{
    _pos.setPosition(x, y);
}

Position& GameObject::getPosition()
{
    return _pos;
}

//moves object on board according to direction user pressed. and updates position of gameobject.
void GameObject::moveGameObject(Position& p, char direction, char symbol, Board board)
{
	if (direction == 's')
		return;

	Position nextPosition(p);

	updateDirection(nextPosition, direction);
	inTunnel(nextPosition, board);

	if (board.getSpot(nextPosition.getX(), nextPosition.getY()) == symbols[(int)symbols::WALL_CHAR] || nextPosition.getX() == board.getTop() - 1 || nextPosition.getX() == board.getHeight()) {
		if (symbol == symbols[(int)symbols::GHOST_CHAR]) {
			if (!_silent) {
				board.gotoxy(p.getX(), p.getY());
				cout << symbols[(int)symbols::GHOST_CHAR];
			}
		}
		return;
	}
		
	else {
		if (!_silent) {
			board.gotoxy(p.getX(), p.getY());
			cout << symbols[(int)symbols::EMPTY_SPACE_CHAR] << endl;
			board.gotoxy(nextPosition.getX(), nextPosition.getY());
			cout << symbol << endl;
		}

		if (board.getSpot(p.getX(), p.getY()) == symbols[(int)symbols::BREADCRUMB_CHAR] && (symbol == symbols[(int)symbols::GHOST_CHAR] || (symbol >= '5' && symbol <= '9'))) {
			if (!_silent) {
				board.gotoxy(p.getX(), p.getY());
				cout << symbols[(int)symbols::BREADCRUMB_CHAR] << endl;
			}
		}
		p.setPosition(nextPosition.getX(), nextPosition.getY());
	}
}

//updates position accoridng to the direction
void GameObject::updateDirection(Position& curr, char direction)
{
	switch (direction)
	{

	case directions[UP]:
		curr.setPosition(curr.getX() - 1, curr.getY());
		break;

	case directions[LEFT]:
		curr.setPosition(curr.getX(), curr.getY() - 1);
		break;

	case directions[DOWN]:
		curr.setPosition(curr.getX() + 1, curr.getY());
		break;

	case directions[RIGHT]:
		curr.setPosition(curr.getX(), curr.getY() + 1);
		break;
	default:
		break;
	}
}

//checks if the position is in a spot of a tunnel opening and sets spot accordingly
void GameObject::inTunnel(Position& p, Board board)
{
	if (p.getX() == board.getTop()) {
		p.setX(board.getHeight() - 1);
		return;
	}
	else if (p.getX() == board.getHeight() - 1) {
		p.setX(board.getTop());
	}
	else if (p.getY() == 0 || p.getY() == board.getWidth() - 1) {
		p.setY(board.getWidth() - 1 - p.getY());
		return;
	}
}

//returns a random direction from directions list
void GameObject::getRandDirection(const Position& p, const Position& pacman, char difficulty, Board board)
{
	bool invaliddirection = false;
	int num;
	Position temp = p;
	Position tempPacman = pacman;

	if (difficulty == BEST || (difficulty == GOOD && timer < 20)) {
		invaliddirection = followPacman(temp, temp.getX() - tempPacman.getX(), temp.getY() - tempPacman.getY(), num, board);
		_direction = directions[num];
	}

	if (((difficulty == NOVICE || difficulty == GOOD ) && timer == 20) || invaliddirection || difficulty == FRUIT) {
		invaliddirection = true;
		while (invaliddirection) {
			temp = p; 
			num = rand() % 4;
				
			updateDirection(temp, directions[num]);
			invaliddirection = invalidNPCMove(temp, board);
		}
		if (difficulty == NOVICE)
			resetTimer();
		_direction = directions[num];
	}
	if (difficulty == GOOD && timer == 25)
		resetTimer();
	updateTimer();
}

//gets direction for the ghost towards pacmans current position
bool GameObject::followPacman(Position& ghost, int diffX, int diffY, int& num, Board board)
{
	bool invaliddirection = true;
	Position temp = ghost;
	num = rand() % 2;
	if (diffX > 0) {
		if (diffY > 0) {
			//up or left
			updateDirection(temp, directions[num]);
			invaliddirection = invalidNPCMove(temp, board);
			if (invaliddirection) {
				temp = ghost;
				num = (num + 1) % 2;
				updateDirection(temp, directions[num]);
				invaliddirection = invalidNPCMove(temp, board);
			}
		}
		else if (diffY < 0) {
			//up or right
			num += (num % 2) + 1;
			updateDirection(temp, directions[num]);
			invaliddirection = invalidNPCMove(temp, board);
			if (invaliddirection) {
				temp = ghost;
				num %= 3;
				num += (num % 2) + 1;
				updateDirection(temp, directions[num]);
				invaliddirection = invalidNPCMove(temp, board);
			}
		}
		else
		{
			//up
			updateDirection(temp, directions[UP]);
			invaliddirection = invalidNPCMove(temp, board);
			num = UP;
		}
	}
	else if (diffX < 0) {
		if (diffY > 0) {
			//down or left
			num += (num % 2);
			updateDirection(temp, directions[num]);
			invaliddirection = invalidNPCMove(temp, board);
			if (invaliddirection) {
				temp = ghost;
				num = (num + 1) % 3;
				num += num % 2;
				updateDirection(temp, directions[num]);
				invaliddirection = invalidNPCMove(temp, board);
			}
		}
		else if (diffY < 0) {
			//down or right
			num += 2;
			updateDirection(temp, directions[num]);
			invaliddirection = invalidNPCMove(temp, board);
			if (invaliddirection) {
				temp = ghost;
				num = (num - 1) % 2;
				num += 2;
				updateDirection(temp, directions[num]);
				invaliddirection = invalidNPCMove(temp, board);
			}
		}
		else
		{
			//down
			updateDirection(temp, directions[DOWN]);
			invaliddirection = invalidNPCMove(temp, board);
			num = DOWN;
		}
	}
	else {
		if (diffY > 0) {
			//left
			updateDirection(temp, directions[LEFT]);
			invaliddirection = invalidNPCMove(temp, board);
			num = LEFT;
		}
		else if (diffY < 0) {
			//right
			updateDirection(temp, directions[RIGHT]);
			invaliddirection = invalidNPCMove(temp, board);
			num = RIGHT;
		}
	}
	return invaliddirection;
}

//checks if the ghost will go to an invalid spot.
bool GameObject::invalidNPCMove(Position p, Board board)
{
	if (board.getSpot(p.getX(), p.getY()) == symbols[(int)symbols::WALL_CHAR] || p.getX() == board.getTop() || p.getX() == board.getHeight() - 1 || p.getY() == 0 || p.getY() == board.getWidth() - 1)
		return true;
	else
		return false;
}

//resets the timer for the ghosts on 'GOOD' or 'NOVICE' difficulty
void GameObject::resetTimer()
{
	timer = 0;
}

//updates the timer for the ghosts on 'GOOD' or 'NOVICE' difficulty
void GameObject::updateTimer()
{
	timer++;
}

//sets the timer for the ghosts on 'GOOD' or 'NOVICE' difficulty
void GameObject::setTimer(const char difficulty)
{
	if (difficulty == NOVICE)
		timer = 20;
}

void GameObject::setSilent(const bool silent)
{
	_silent = silent;
}
