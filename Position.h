/* This Class holds 2 coordinates (x and y). */

#pragma once
#ifndef __POSITION_H
#define __POSITION_H

#include <iostream>

//using namespace std;

class Position
{
private: 
	int _x, _y;

public:
	const int getX();
	const int getY();
	void setX(const int x);
	void setY(const int y);
	void setPosition(const int x, const  int y);

	Position(int x = 0, int y = 0);
	Position(const Position& other);
};

#endif // !__POSITION_H
