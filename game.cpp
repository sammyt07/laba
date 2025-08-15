/**
 * game.cpp
 * Authors: Sammy T.
 * Implementation of game.hpp
 * 
 * This is Laba, a 2D, puzzle game with ASCII graphics
 * inspired by games like Dwarf Fortress and Portal.
 */

#include "game.hpp"

#include <gba.h>
#include <stdio.h>

void drawPlayer(int row, int col) {
    currLvl_[row][col] = '@';
    iprintf("\x1b[%d;%dH@", playerRow_, playerCol_);
}

void redrawPlayer(int drow, int dcol) {    
    // ---------------------- movement ------------------------ //
    int newRow = playerRow_+drow;
    int newCol = playerCol_+dcol;

    // bounds check
    if (newRow < 0 || newRow >= MAP_HEIGHT || newCol < 0 || newCol >= MAP_WIDTH-1) {
        return;
    }

    // wall collision check
    if (currLvl_[newRow][newCol] == '#') {
        return;
    }

    // obstacle collision check
    if (currLvl_[newRow][newCol] == '-') {
        return;
    }

    // cube collision check
    if (currLvl_[newRow][newCol] == '[' || currLvl_[newRow][newCol] == ']') {
        return;
    }

    // ---------------------- drawing ------------------------ //
    // erase old player
    currLvl_[playerRow_][playerCol_] = '.';
    iprintf("\x1b[%d;%dH.", playerRow_, playerCol_);  // GBA cursor is 1-based

    // update position
    playerRow_ = newRow;
    playerCol_ = newCol;

    // draw new position
    drawPlayer(playerRow_, playerCol_);
    iprintf("\x1b[%d;%dH@", playerRow_, playerCol_);
}

void drawLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]) {
    // prevent drawing player on title screen
    if (!showTitleScreen && !showEndScreen) {
        drawPlayer(playerRow_, playerCol_);  // draw player at start position
    }

    iprintf("\x1b[2J");  // optional: clear screen first
    for (int x = 0; x < MAP_HEIGHT; x++) {
        for (int y = 0; y < MAP_WIDTH - 1; y++) {  // -1 to ignore null terminator
            char tile = lvl[x][y];
            iprintf("%c", tile);
        }
        iprintf("\n");
    }
}

void switchLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]) {
    // copy tiles from lvl to currLvl_
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            currLvl_[i][j] = lvl[i][j];
        }
    }
    drawLvl(currLvl_);  // draw map and player
}

// notes: turrets that fire if player is in their col (basic AI), portal gun

void interact() {
    if (playerCol_ + 2 >= MAP_WIDTH - 1) return;  // needs space for [ and ]

    char east = currLvl_[playerRow_][playerCol_+1];  // check player right

    switch (east) {
        case '[':  // cube is to the right
            if (!interacted_) {  // cube is not in player inventory
                if (currLvl_[playerRow_][playerCol_+3] == '-') {  // cube is partially on platform
                    // draw empty space and platform
                    currLvl_[playerRow_][playerCol_+1] = '.';
                    currLvl_[playerRow_][playerCol_+2] = '-';

                    // erase cube from screen
                    iprintf("\x1b[%d;%dH.", playerRow_, playerCol_+1);
                    iprintf("\x1b[%d;%dH-", playerRow_, playerCol_+2);
                }
                else if (currLvl_[playerRow_][playerCol_+3] == '-') {

                }  
                else { // cube is in empty space
                    // add empty space to map
                    currLvl_[playerRow_][playerCol_+1] = '.';
                    currLvl_[playerRow_][playerCol_+2] = '.';

                    // erase cube from screen
                    iprintf("\x1b[%d;%dH.", playerRow_, playerCol_+1);
                    iprintf("\x1b[%d;%dH.", playerRow_, playerCol_+2);
                }

                interacted_ = true;  // update status: add cube to player inventory
            }
            break;
        case '-':  // platform is to the right
            if (interacted_) {  // place cube
                
                // add cube to map
                currLvl_[playerRow_][playerCol_+1] = '[';
                currLvl_[playerRow_][playerCol_+2] = ']';

                // draw cube on screen
                iprintf("\x1b[%d;%dH[", playerRow_, playerCol_+1);
                iprintf("\x1b[%d;%dH]", playerRow_, playerCol_+2);

                // win state: switch levels
                iprintf("\x1b[2J");  // optional: clear screen first
                switch (lvlIndex_) {
                    case 0:
                        // new start pos
                        playerRow_ = 12;
                        playerCol_ = 17;
                        lvlIndex_++;
                        switchLvl(lvl2_);
                        break;
                    case 1:
                        // new start pos
                        playerRow_ = 3;
                        playerCol_ = 15;
                        lvlIndex_++;
                        switchLvl(lvl3_);
                        break;
                    case 2:
                        showEndScreen = true;
                        lvlIndex_++;
                        switchLvl(end_);
                        break;
                }

                interacted_ = false;  // update status: remove cube from player inventory
            } else {

            }
            break;
        case '.':  // empty space is to the right
            if (interacted_) {  // place cube
                // add cube to map
                currLvl_[playerRow_][playerCol_+1] = '[';
                currLvl_[playerRow_][playerCol_+2] = ']';

                // draw cube on screen
                iprintf("\x1b[%d;%dH[", playerRow_, playerCol_+1);
                iprintf("\x1b[%d;%dH]", playerRow_, playerCol_+2);

                interacted_ = false;  // update status: remove cube from player inventory
            }
            break;
    }
}

int main() {
    // Init interrupts and console system
    irqInit();
    irqEnable(IRQ_VBLANK);
    consoleDemoInit();  // Initializes MODE_0 with BG0 text background
    BG_PALETTE[0] = RGB5(0,0,0);  // set black background

    // Initialize game
    interacted_ = false;
    showTitleScreen = true;
    switchLvl(intro_);  // set level to first level

    while (1) {
        scanKeys();
        u16 held = keysHeld();
        u16 down = keysDown();

        // handle game start and end
        if (down & KEY_START) {
            if (lvlIndex_ < 3) {
                showTitleScreen = false;
                switchLvl(lvl1_);
            } else {
                // reset game and player pos
                switchLvl(intro_);
                showEndScreen = false;
                lvlIndex_ = 0;
                playerRow_ = 12;
                playerCol_ = 10;
            }
        }

        // basic movement with D-pad
        if (!showTitleScreen && !showEndScreen) {
            if (held & KEY_UP)    redrawPlayer(-1,0);
            if (held & KEY_DOWN)  redrawPlayer(1,0);
            if (held & KEY_LEFT)  redrawPlayer(0,-1);
            if (held & KEY_RIGHT) redrawPlayer(0,1);

            // handle cube pickup or press with A button
            if (down & KEY_A) interact();
        }

        for (volatile int i = 0; i < 100000; i++);  // crude delay

        VBlankIntrWait();
    }

    return 0;
}
