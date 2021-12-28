/* This is the cpp file of Position. (look at header file for description of Class.) */

#include "Position.h"

const int Position::getX()
{
	return _x;
}

const int Position::getY()
{
	return _y;
}

void Position::setX(const int x)
{
	this->_x = x;
}

void Position::setY(const int y)
{
	this->_y = y;
}

//sets x and y in one call.
void Position::setPosition(const int x, const  int y)
{
	setX(x);
	setY(y);
}

//constructor
Position::Position(int x, int y): _x(x), _y(y) {}

//Copy Constructor
Position::Position(const Position& other): _x(other._x), _y(other._y) {}
