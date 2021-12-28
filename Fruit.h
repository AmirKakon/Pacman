/* This Class holds fruits value (random value) and timers for fruit */

#pragma once

#include "GameObject.h"

#define FRUIT_TIMER 50

class Fruit: public GameObject
{
private:
	int _value = 0;
	char _symbol;
	int timer = 0;
	bool onboard = false;

public:
	void setValue(int value);
	void setSymbol(char symbol);
	const int getValue();
	const char getSymbol();
	const bool getState();
	void changestate();
	void resettimer();
	void updatetimer();
	bool timetodrop();
	bool timesup();
};

