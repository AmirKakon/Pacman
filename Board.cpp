/* This is the cpp file of Board. (look at header file for description of Class.) */

#include "Board.h"

//sets up board to begining position with all breadcrumbs showing.
void Board::updateStats(const int points,const int lives)
{
	gotoxy(_stats.getX(), _stats.getY());
	cout << "Points: " << points;
	gotoxy(_stats.getX()+1, _stats.getY());
	cout << "Lives: " << lives;
}

//reads and updates board from a specific file
int Board::readBoardFromFile(Position p[], int& count, string fileName)
{
	_stats.setPosition(-1, -1);
	p[0].setPosition(0, 0);
	fstream f(fileName, ios::in);
	
	if (!checkFile(f)) {
		throw 0;
	}
	char c;
	int i = 0, j = 0;
	int maxBreadCrumbs = 0;
	bool onboard = false;
	bool gottop = false;

	c = f.get();

	while (c !=EOF) {
		
		if (c == '\n') {
			if (i == 0)
				_width = j;
			if(i >=80) {
				throw "Board height is too big";
			}
			i++;
			j = 0; 
		}
		else {

			if (j >= 25) {
				throw"Board width is too big";
			}

			switch (c) {
			case symbols[(int)symbols::EMPTY_SPOT_CHAR]:
				_board[i][j] = symbols[(int)symbols::EMPTY_SPACE_CHAR];
				break;
			case symbols[(int)symbols::EMPTY_SPACE_CHAR]:
				_board[i][j] = symbols[(int)symbols::BREADCRUMB_CHAR];
				maxBreadCrumbs++;
				break;
			case  symbols[(int)symbols::GHOST_CHAR]:
				_board[i][j] = symbols[(int)symbols::GHOST_CHAR];
				p[++count].setPosition(i, j);
				if (count == 5) {
					throw "Too many ghosts on board.";
				}
				break;
			case symbols[(int)symbols::PACMAN_CHAR]:
				_board[i][j] = symbols[(int)symbols::PACMAN_CHAR];
				if (p[0].getX() != 0 && p[0].getY() != 0) {
					throw "Too many Pacmans on board.";
				}
				p[0].setPosition(i, j);
				break;
			case symbols[(int)symbols::STATS_CHAR]:
				_board[i][j] = symbols[(int)symbols::STATS_CHAR];
				if (_stats.getX() != -1 && _stats.getY() != -1) {
					throw "Too many stats on board.";
				}
				_stats.setPosition(i, j);
				break;
			case symbols[(int)symbols::WALL_CHAR]:
				_board[i][j] = symbols[(int)symbols::WALL_CHAR];
				break;
			default:
				throw "Illegal character on board.";
				break;
			}

			if (j == 0) {
				if (!onboard && c == symbols[(int)symbols::WALL_CHAR] && !gottop) {
					_top = i;
					onboard = true;
					gottop = true;
				}
			}
			j++;
		}

		c = f.get();

		if (j == 0) {

			if (onboard && c != symbols[(int)symbols::WALL_CHAR]) {
				if (!checkNextLine(f, f.tellg())) {
					_bottom = i - 1;
					onboard = false;
				}
			}
		}
	}

	if (onboard) {
		_bottom = i;
		_height = i + 1;
	}
	else
		_height = i;

	if (_height == 0) {
		throw "Board is empty";
	}

	if (_stats.getY() >= 12) {
		throw "Not enought room for stats on screen";
	}

	f.close();

	if (p[0].getX() == 0) {
		throw "No Pacman on board.";
	}

	if (_stats.getX() == -1) {
		throw "No stats on board.";
	}

	return maxBreadCrumbs;
}

//checks that file exists
bool Board::checkFile(fstream& file)
{
	if(!file) {
		cout << "File not found." << endl;
		return false;
	}
	return true;
}

//prints board
void Board::printBoard(const int& points, const int& lives)
{
	gotoxy(0, 0);

	int count = 1;

	for (int i = 0; i < _height; ++i) {
		for (int j = 0; j < _width; ++j) {
			cout << _board[i][j];
		}
		cout << endl;
	}
}

//gets char of spot
const char Board::getSpot(int i, int j)
{
	return _board[i][j];
}

//sets char on spot
void Board::setSpot(int i, int j, char symbol)
{
	_board[i][j] = symbol;
}

//function from class - sets cursor at (x,y) location
void Board::gotoxy(short x, short y)
{
	static HANDLE hConsoleOutput = NULL;
	if (NULL == hConsoleOutput) //initializer which will only be true the first time (b/c its a static)
		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwCursorPosition = { y, x }; //coordinate with x,y positions
	cout.flush(); //clers any extra presses from the keyboard
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

//gets all files from folder
void Board::getFiles()
{
	string path;

	for (const auto& i : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		path = i.path().filename().u8string();
		if (path.size() > 11 &&  path.rfind(".screen") == 9) {
			_screens.push_back(path);
		}
	}
}

//gets number of screens available
const int Board::getNumberOfScreens()
{
	return _screens.size();
}

//gets specific screen name
const string Board::getScreen(int index)
{
	return _screens[index];
}

//checks if next lines are a tunnel
bool Board::checkNextLine(fstream& f, int index)
{
	int j=1;
	char c = f.get();

	while (c != EOF) {
		if (c == '\n') {
			j = -1;
		}
		c = f.get();
		j++;
		if (j == 0 && c == symbols[(int)symbols::WALL_CHAR]) {
			f.seekg(index);
			return true;
		}
			
	}
	f.seekg(index);
	return false;
}


const int Board::getHeight()
{
	return _height;
}

const int Board::getWidth()
{
	return _width;
}

//gets top dimensions of physical board
const int Board::getTop()
{
	return _top;
}

//gets bottom dimensions of physical board
const int Board::getBottom()
{
	return _bottom;
}

void Board::addScreen(string name)
{
	_screens.push_back(name);
}

void Board::clearScreens()
{
	_screens.clear();
}
