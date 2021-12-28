/* This is the cpp file of Fruit. (look at header file for description of Class.) */
#include "Fruit.h"

void Fruit::setValue(int value)
{
	this->_value = value;
}

void Fruit::setSymbol(char symbol)
{
	this->_symbol = symbol;
}

const int Fruit::getValue()
{
	return _value;
}

const char Fruit::getSymbol()
{
	return _symbol;
}

const bool Fruit::getState()
{
	return onboard;
}

//changes state of fruit to be on board or not
void Fruit::changestate()
{
	onboard = !onboard;
}

//ressets the timer for the fruit
void Fruit::resettimer()
{
	timer = 0;
}

//upadates the timer based on the state
void Fruit::updatetimer()
{
	if (onboard)
		timer -= 1;
	else
		timer += 1;
}

//checks if time for fruit to drop
bool Fruit::timetodrop()
{
	return (timer == FRUIT_TIMER && !onboard);
}

//checks if time is up for the fruit
bool Fruit::timesup()
{
	return (timer == 0 && onboard);
}
