/**
 * game.cpp
 * Authors: Sammy T.
 * Implementation of game.hpp
 * 
 * This is Lambda Complex, a 2D, puzzle game.
 */

#include "game.hpp"

#include <gba.h>
#include <stdio.h>

void drawPlayer(int row, int col) {
    lvl1_[row][col] = '@';
}

void redrawPlayer(int drow, int dcol) {    
    // ---------------------- movement ------------------------ //
    int newRow = playerRow_+drow;
    int newCol = playerCol_+dcol;

    // bounds check
    if (newRow < 0 || newRow > MAP_HEIGHT || newCol < 0 || newCol >= MAP_WIDTH) {
        return;
    }

    // wall collision check
    if (lvl1_[newRow][newCol] == '#') {
        return;
    }

    // obstacle collision check
    if (lvl1_[newRow][newCol] == '-') {
        return;
    }

    // cube collision check
    if (lvl1_[newRow][newCol] == '[' || lvl1_[newRow][newCol] == ']') {
        return;
    }

    // ---------------------- drawing ------------------------ //
    // erase old player
    lvl1_[playerRow_][playerCol_] = '.';
    iprintf("\x1b[%d;%dH.", playerRow_, playerCol_);  // GBA cursor is 1-based

    // update position
    playerRow_ = newRow;
    playerCol_ = newCol;

    // draw new position
    drawPlayer(playerRow_, playerCol_);
    iprintf("\x1b[%d;%dH@", playerRow_, playerCol_);
}

void drawLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]) {
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
    // copy map data
    for (int i = 0; i < MAP_HEIGHT; i++) {
        currLvl_[i][MAP_WIDTH-1] = lvl[i][MAP_WIDTH-1];
    }
    drawLvl(currLvl_);
}

// somme ideas: turrets that fire if player is in their col (basic AI), portal gun

void interact() {
    char east = lvl1_[playerRow_][playerCol_+1];  // check player right

    switch (east) {
        case '[':  // cube is to the right
            if (!interacted_) {  // cube is not in player inventory
                if (lvl1_[playerRow_][playerCol_+3] == '-') {  // cube is partially on platform
                    // draw empty space and platform
                    lvl1_[playerRow_][playerCol_+1] = '.';
                    lvl1_[playerRow_][playerCol_+2] = '-';

                    // erase cube from screen
                    iprintf("\x1b[%d;%dH.", playerRow_, playerCol_+1);
                    iprintf("\x1b[%d;%dH-", playerRow_, playerCol_+2);
                }
                else if (lvl1_[playerRow_][playerCol_+3] == '-') {

                }  
                else { // cube is in empty space
                    // add empty space to map
                    lvl1_[playerRow_][playerCol_+1] = '.';
                    lvl1_[playerRow_][playerCol_+2] = '.';

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
                lvl1_[playerRow_][playerCol_+1] = '[';
                lvl1_[playerRow_][playerCol_+2] = ']';

                // draw cube on screen
                iprintf("\x1b[%d;%dH[", playerRow_, playerCol_+1);
                iprintf("\x1b[%d;%dH]", playerRow_, playerCol_+2);

                // win state: player moves to next level
                interacted_ = false;  // update status: remove cube from player inventory
            } else {
                
            }
            break;
        case '.':  // empty space is to the right
            if (interacted_) {  // place cube
                // add cube to map
                lvl1_[playerRow_][playerCol_+1] = '[';
                lvl1_[playerRow_][playerCol_+2] = ']';

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
    playerRow_ = 12;  // player start row
    playerCol_ = 10;  // player start col
    drawPlayer(playerRow_, playerCol_);
    drawLvl(lvl1_);

    while (1) {
        scanKeys();
        u16 held = keysHeld();
        u16 down = keysDown();

        // basic movement with D-pad
        if (held & KEY_UP)    redrawPlayer(-1,0);
        if (held & KEY_DOWN)  redrawPlayer(1,0);
        if (held & KEY_LEFT)  redrawPlayer(0,-1);
        if (held & KEY_RIGHT) redrawPlayer(0,1);

        // handle cube pickup or press with A button
        if (down & KEY_A) interact();

        for (volatile int i = 0; i < 100000; i++);  // crude delay

        VBlankIntrWait();
    }

    return 0;
}