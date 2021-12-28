/* This Class creates and pritns board according to files.*/

#pragma once
#include <iostream>
#include "Symbols.h"
#include "Position.h"
#include <fstream>
#include <windows.h>
#include <vector>
#include <string>
#include <filesystem>  

using namespace std;

#define MAX_WIDTH 25 //max width of board
#define MAX_HEIGHT 80 //max heighgt of board

class Board
{
private:
	int width = 0, height = 0;
	char _board[MAX_HEIGHT][MAX_WIDTH];
	Position _stats;
	int _height, _width;
	int _top, _bottom;
	bool _addcolor;
	vector<string> _screens;

public: 
	void updateStats(const int points, const int lives);
	int readBoardFromFile(Position p[], int& count, string fileName);
	bool checkFile(fstream& file);
	void printBoard(const int& points, const int& lives);
	const char getSpot(int i, int j);
	void setSpot(int i, int j, char symbol);
	void gotoxy(short x, short y);
	void getFiles();
	const int getNumberOfScreens();
	const string getScreen(int index);
	bool checkNextLine(fstream& f, int index);
	const int getHeight();
	const int getWidth();
	const int getTop();
	const int getBottom();
};