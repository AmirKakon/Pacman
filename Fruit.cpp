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
	ftimer = 0;
}

//upadates the timer based on the state
void Fruit::updatetimer()
{
	if (onboard)
		ftimer -= 1;
	else
		ftimer += 1;
}

//checks if time for fruit to drop
bool Fruit::timetodrop()
{
	return (ftimer == FRUIT_TIMER && !onboard);
}

//checks if time is up for the fruit
bool Fruit::timesup()
{
	return (ftimer == 0 && onboard);
}
