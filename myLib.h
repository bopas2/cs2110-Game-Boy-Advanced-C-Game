#ifndef MYLIB_H
#define MYLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "gba.h"
#include "images/evil_ship.h"
#include "images/gamebg.h"
#include "images/player.h"
#include "images/startScreen.h"
#include "images/gamebg.h"
#include "images/endgame.h"
#include "images/instructions.h"
#include "images/controls.h"

void drawProjectiles(Game *GAMESTATE);
void drawEnemies(Game *GAMESTATE);
void drawPlayer(Game *GAMESTATE);
void drawText(Game *GAMESTATE);
void drawScores(Game *GAMESTATE);
void resetGame(Game *GAMESTATE);
void spawnEnemies(Game *GAMESTATE);
void playerInput(Game *GAMESTATE, u32 previousButtons, u32 currentButtons);
void drawGame(Game *GAMESTATE);

#endif