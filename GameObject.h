/* This Class holds a position and a char as direction which indicates the direction the GameObject is heading. (according to w,a,s,d,x). */

#pragma once
#ifndef __GAMEOBJECT_H
#define __GAMEOBJECT_H

#include "Position.h"
#include "Board.h"

#define LEFT 0
#define UP 1
#define DOWN 2
#define RIGHT 3
#define STAY 4
#define ESC 27
#define BEST 'a'
#define GOOD 'b'
#define NOVICE 'c'
#define FRUIT 'd'

class GameObject
{
	private:
		Position _pos;
		char _direction = 's';
		int timer = 0;
		void updateDirection(Position& curr, char direction);
		void inTunnel(Position& p, Board board);
		bool invalidNPCMove(Position p, Board board);
		static bool _silent;

	public:
		static constexpr char directions[] = { 'a', 'w', 'x', 'd', 's' }; // all possible directions
		const char getDirection();
		void setDirection(const char direction);
		void setPosition(const int x, const  int y);
		Position& getPosition();
		void moveGameObject(Position& p, char direction, char symbol, Board board);
		void getRandDirection(const Position& p, const Position& pacman, char difficulty, Board board);
		bool followPacman(Position& ghost, int diffX, int diffY, int& num, Board board);
		void resetTimer();
		void updateTimer();
		void setTimer(const char difficulty);
		void setSilent(const bool silent);
};

#endif // !__GAMEOBJECT_H



