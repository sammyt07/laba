#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

/**
 * \brief Laba! is a 2D, ASCII puzzle game inspired by games like Dwarf Fortress and Portal.
 */
// ------------ variables ------------- //
static constexpr int MAP_HEIGHT = 19;  // height limit on GBA
static constexpr int MAP_WIDTH = 30;   // width limit on GBA

int playerRow_ = 12;  // player start pos
int playerCol_ = 10;  // player start pos
int turretRow_ = 10;  // turret pos in lvl1_
int turretCol_ = 25;  // turret pos in lvl1_
int lvlIndex_ = 0;
int bulletRow_ = 0;
int bulletCol_ = 0;
int bulletDir_ = 0;
bool interacted_ = false;
bool showTitleScreen_ = true;
bool showEndScreen_ = false;
bool bulletActive_ = false;
bool turretActive_ = true;
bool isAlive_ = true;

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
char dead_[MAP_HEIGHT][MAP_WIDTH] = {
    "#############################",
    "#                           #",
    "#                           #",
    "#                           #",
    "#            You            #",
    "#                           #",
    "#          D I E D          #",
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
 * \brief     Draws the player on the game map at (row, col).
 * \param row index for the row of the player on the game map.
 * \param col index for the col of the player on the game map.
 */
void drawPlayer(int row, int col);

/**
 * \brief      Redraws the player to update their position.
 * \param drow int indicating the change in row of the player.
 * \param dcol int indicating the change in col of the player.
 */
void redrawPlayer(int drow, int dcol);

/**
 * \brief     Draws a turret NPC on the game map at (row, col).
 * \param row index for the row of the turret on the game map.
 * \param col index for the col of the turret on the game map.
 */
void drawTurret(int row, int col);

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

/**
 * \brief Scans the left and right of the turret, firing if the
 *        player is within line of sight.
 */
void scanLeftRight();

/**
 * \brief      Fires bullets from the turret along its line of sight.
 * \param row  index for the row position of the bullet on the game map.
 * \param col  index for the col position of the bullet on the game map.
 * \param dir  Direction of turret line of sight.
 */
void startBullet(int row, int col, int dir);

/**
 * \brief Redraws the bullet along its path of motion.
 */
void redrawBullet();

/**
 * \brief Handles player health and combat.
 */
void playerHit();

#endif
