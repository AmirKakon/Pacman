/*This file contains all the symbols for the pacman board.*/
#pragma once

static constexpr char symbols[] = { '@', '$', '#', '&', '*', '%', ' ' }; //all symbols that will be shown on board

enum class symbols {
PACMAN_CHAR = 0,
GHOST_CHAR =1,
WALL_CHAR =2,
STATS_CHAR =3,
BREADCRUMB_CHAR =4,
EMPTY_SPOT_CHAR =5,
EMPTY_SPACE_CHAR =6
};