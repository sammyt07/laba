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
    iprintf("\x1b[%d;%dH@", row, col);
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

    // turret collision check - combat
    if (currLvl_[newRow][newCol] == 'X') {
        turretActive_ = false;
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

void drawTurret(int row, int col) {
    currLvl_[row][col] = 'X';
    iprintf("\x1b[%d;%dHX", row, col);
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

    // prevent drawing player on title screen
    if (!showTitleScreen_ && !showEndScreen_ && isAlive_) {
        drawPlayer(playerRow_, playerCol_);  // draw player at start position
        drawTurret(turretRow_, turretCol_);  // draw turret
    }
}

void switchLvl(char lvl[MAP_HEIGHT][MAP_WIDTH]) {
    turretActive_ = true;
    bulletActive_ = false;

    // copy tiles from lvl to currLvl_
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            currLvl_[i][j] = lvl[i][j];
        }
    }
    drawLvl(currLvl_);  // draw map and player
}

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
                    // huh???? nothing here...
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
                        playerRow_ = 12;
                        playerCol_ = 17;
                        lvlIndex_++;
                        turretActive_ = false;
                        switchLvl(lvl2_);
                        break;
                    case 1:
                        playerRow_ = 3;
                        playerCol_ = 15;
                        lvlIndex_++;
                        turretActive_ = false;
                        switchLvl(lvl3_);
                        break;
                    case 2:
                        playerRow_ = 12;
                        playerCol_ = 10;
                        lvlIndex_++;
                        turretActive_ = false;
                        showEndScreen_ = true;
                        switchLvl(end_);
                        break;
                }

                interacted_ = false;  // update status: remove cube from player inventory
            } else {
                // huh nothing here either? i gotta look at this again lol
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

void scanLeftRight() {
    // check left
    for (int i = turretCol_ - 1; i > 0; --i) {
        char tile = currLvl_[turretRow_][i];

        // found walls or weighted cube
        if (tile == '#' || tile == '[' || tile == ']') break;

        // found player
        if (tile == '@') {
            startBullet(turretRow_, turretCol_, -1);  // -1 ~ shoot to the left
            break;
        }
    }
}

void startBullet(int row, int col, int dir) {
    if (bulletActive_) return;  // prevent multiple bullets at a time
    bulletActive_ = true;
    bulletRow_ = row;
    bulletCol_ = col + dir;     // spawn just in front (L / R) of the turret
    bulletDir_ = dir;
    iprintf("\x1b[%d;%dH-", bulletRow_, bulletCol_);  // draw bullet
}

void redrawBullet() {
    if (!bulletActive_) return;

    int nextCol = bulletCol_ + bulletDir_;

    // collision check with bounds
    if (nextCol <= 0 || nextCol >= MAP_WIDTH - 1) {
        iprintf("\x1b[%d;%dH.", bulletRow_, bulletCol_);
        bulletActive_ = false;
        return;
    }

    char ahead = currLvl_[bulletRow_][nextCol];

    // collision check with wall or box
    if (ahead == '#' || ahead == ']') {
        iprintf("\x1b[%d;%dH.", bulletRow_, bulletCol_);
        bulletActive_ = false;
        return;
    }

    // collision check with player
    if (currLvl_[bulletRow_][bulletCol_] == '@' || ahead == '@') {  // fixes teleporting through bullets?
        iprintf("\x1b[%d;%dH.", bulletRow_, bulletCol_);
        bulletActive_ = false;
        playerHit();
        return;
    }

    iprintf("\x1b[%d;%dH.", bulletRow_, bulletCol_);  // erase bullet in previous pos
    bulletCol_ = nextCol;
    iprintf("\x1b[%d;%dH-", bulletRow_, bulletCol_);  // draw bullet in new pos
}

void playerHit() {
    // erase player, reset, and trigger game over
    isAlive_ = false;
    iprintf("\x1b[%d;%dH ", playerRow_, playerCol_);
    currLvl_[playerRow_][playerCol_] = '.';
    drawLvl(dead_);
}

int main() {
    // Init interrupts and console system
    irqInit();
    irqEnable(IRQ_VBLANK);
    consoleDemoInit();  // Initializes MODE_0 with BG0 text background
    BG_PALETTE[0] = RGB5(0,0,0);  // set black background

    // Initialize game
    switchLvl(intro_);  // set level to first level

    while (1) {
        scanKeys();
        u16 held = keysHeld();
        u16 down = keysDown();

        if (turretActive_) scanLeftRight();  // handle NPC behavior

        if (down & KEY_START) {
            if (lvlIndex_ < 3) {
                if (isAlive_) {
                    // restart game normally
                    showTitleScreen_ = false;
                    switchLvl(lvl1_);
                } else {
                    // player died: reset player pos and alive status, return to title screen
                    playerRow_ = 12;
                    playerCol_ = 10;
                    isAlive_ = true;
                    showTitleScreen_ = true;
                    switchLvl(intro_);
                }
            } else {
                // win state - player finished game: reset game, return to title screen
                lvlIndex_ = 0;
                isAlive_ = true;
                showEndScreen_ = false;
                switchLvl(intro_);
            }
        }

        // basic movement with D-pad
        if (isAlive_) {
            if (!showTitleScreen_ && !showEndScreen_) {
                if (held & KEY_UP)    redrawPlayer(-1,0);
                if (held & KEY_DOWN)  redrawPlayer(1,0);
                if (held & KEY_LEFT)  redrawPlayer(0,-1);
                if (held & KEY_RIGHT) redrawPlayer(0,1);

                // handle pickup / place cube with A
                if (down & KEY_A) interact();
            }
        }

        for (volatile int i = 0; i < 100000; i++);  // crude delay

        redrawBullet();  // bullet advances one step per frame in-game

        VBlankIntrWait();
    }

    return 0;
}
