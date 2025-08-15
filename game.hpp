#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

/**
 * \brief laba is a 2D, ASCII puzzle game inspired by games like Dwarf Fortress and Portal.
 */
// ------------ variables ------------- //
static constexpr int MAP_HEIGHT = 19;  // height limit on GBA
static constexpr int MAP_WIDTH = 30;   // width limit on GBA

int playerRow_ = 12;  // lvl1_ start
int playerCol_ = 10;  // lvl1_ start
int lvlIndex_ = 0;
bool interacted_ = false;
bool showTitleScreen = false;
bool showEndScreen = false;

char intro_[MAP_HEIGHT][MAP_WIDTH] = {
    "#############################",
    "#                           #",
    "#                           #",
    "#                           #",
    "#            Laba!          #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#         Press A to        #",
    "#       pick up / place     #",
    "#          boxes []         #",
    "#                           #",
    "#        Use D-Pad to       #",
    "#            move           #",
    "#                           #",
    "#       Press Start to      #",
    "#            Play!          #",
    "#                           #",
    "#############################"
};
char lvl1_[MAP_HEIGHT][MAP_WIDTH] = {
    "#############################",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........--..............#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#.................[]........#",
    "#...........................#",
    "#...........................#",
    "#############################"
};
char lvl2_[MAP_HEIGHT][MAP_WIDTH] = {
    "#############################",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#.....[]....................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#..........--...............#",
    "#...........................#",
    "#############################"
};
char lvl3_[MAP_HEIGHT][MAP_WIDTH] = {
    "#############################",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#.............--............#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#...........................#",
    "#..[].......................#",
    "#...........................#",
    "#...........................#",
    "#############################"
};
char end_[MAP_HEIGHT][MAP_WIDTH] = {
    "#############################",
    "#                           #",
    "#                           #",
    "#                           #",
    "#            Laba!          #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#       Press Start to      #",
    "#         to Restart        #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#                           #",
    "#############################"
};
char currLvl_[MAP_HEIGHT][MAP_WIDTH];

// ------------ rendering ------------- //
/**
 * \brief      Draws the 2D map array.
 * \param lvl  2D character-representation of the game map.
 */
void drawLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]);
        
/**
 * \brief     Draws the player on the map at the (row, col).
 * \param row index for the row of the player on the game map.
 * \param col index for the col of the player on the game map.
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
 * \brief            Switches to the next level upon player win state.
 * \param lvl        2D character-representation of the game map.
 */
void switchLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]);

#endif
