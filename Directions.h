/*This file contains all the directions for the pacman board.*/
#pragma once

static constexpr char directions[] = { 'a', 'w', 'x', 'd', 's' }; // all possible directions

enum class directions {
LEFT = 0,
UP = 1,
DOWN = 2,
RIGHT = 3,
STAY = 4
};
