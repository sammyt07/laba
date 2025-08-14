#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

/**
 * \brief Lambda Complex is a 2D, ASCII puzzle game.
 */
// ------------ variables ------------- //
static constexpr int MAP_HEIGHT = 16;
static constexpr int MAP_WIDTH = 25;

int playerRow_;
int playerCol_;
bool interacted_ = false;
char currLvl_[MAP_HEIGHT][MAP_WIDTH];
char lvl1_[MAP_HEIGHT][MAP_WIDTH] = {
    "########################",
    "#......................#",
    "#......................#",
    "#......................#",
    "#...........--.........#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#................[]....#",
    "#......................#",
    "#......................#",
    "########################"
};
char lvl2_[MAP_HEIGHT][MAP_WIDTH] = {
    "########################",
    "#......................#",
    "#......................#",
    "#......................#",
    "#.....[]...............#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#......................#",
    "#..........--..........#",
    "#......................#",
    "########################"
};

// ------------ rendering ------------- //
/**
 * \brief      Draws the 2D map array.
 * \param lvl  2D character-representation of the game map.
 */
void drawLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]);
        
/**
 * \brief     Draws the player on the map at the (row, col).
 * \param row int index for the row of the player on the map.
 * \param col int index for the col of the player on the map.
 */
void drawPlayer(int row, int col);

/**
 * \brief      Redraws the player to update their position.
 * \param drow int index for the new row of the player.
 * \param dcol int index for the new col of the player.
 */
void redrawPlayer(int drow, int dcol);

// ------------ game logic ------------- //
/**
 * \brief Handles picking up / placing weighted cubes depending
 *        on "interacted."
 */
void interact();

/**
 * \brief Switches to the next level upon player win state.
 * \param lvl 2D character-representation of the game map.
 */
void switchLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]);

#endif