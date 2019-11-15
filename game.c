#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gba.h"
#include "game.h"
#include "images/startScreen.h"
#include "images/garbage.h"
#include "images/player.h"
#include "images/evil_ship.h"
#include "images/gamebg.h"
#include "images/endgame.h"

typedef enum {
    START,
    PLAY,
    END,
} GBAState;

int main(void) {
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    // Save current and previous state of button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    // Load initial game state
    GBAState state = START;

    Player PlayerShip = {100, 50, 50, 17, 17, 3, 0, 10};
    int MAX_ENEMIES = 30;
    Enemy Enemies[MAX_ENEMIES];
    int EnemiesAlive = 0;
    int lastAttack = 0;

    int MAX_PROJECTILES = 15;
    Bullet Bullets[MAX_PROJECTILES];

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        Bullet s = {0, 0, 0, 0, 0, 0, 0};
        Bullets[i] = s;
    }

    while (1) {
        currentButtons = BUTTONS; // Load the current state of the buttons
        waitForVBlank();

        switch(state) {
            case START:
                drawFullScreenImageDMA(start_screen);
                PlayerShip.score = 0;
                PlayerShip.health = 100;
                EnemiesAlive = 0;
                for (int e = 0; e < MAX_ENEMIES; e++) {
                    Enemies[e].health = 0;
                }
                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_A, currentButtons)) {
                        state = PLAY;
                        drawFullScreenImageDMA(gamebg);
                        break;
                    }
                }
                break;
            case PLAY:

                // RESET CASE
                if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        break;
                    }
                }

                // PLAYER MOVEMENT
                drawImageDMA(PlayerShip.x, PlayerShip.y, PlayerShip.width, PlayerShip.height, player);
                if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
                    if (PlayerShip.x + PlayerShip.speed + 2*PlayerShip.width < 260) {
                        PlayerShip.x += PlayerShip.speed;
                    }
                }
                if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
                    if (PlayerShip.x - PlayerShip.speed >= 0) {
                        PlayerShip.x -= PlayerShip.speed;
                    }
                }
                if (KEY_DOWN(BUTTON_UP, currentButtons)) {
                    if (PlayerShip.y - PlayerShip.speed >= 0) {
                        PlayerShip.y -= PlayerShip.speed;
                    }
                }
                if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
                    if (PlayerShip.y + PlayerShip.speed + PlayerShip.height < 160) {
                        PlayerShip.y += PlayerShip.speed;
                    }
                }

                // PLAYER ATTACK 
                if (KEY_JUST_PRESSED(BUTTON_B, currentButtons, previousButtons)) {
                    for (int i = 0; i < MAX_PROJECTILES; i++) {
                        if (Bullets[i].alive == 0) {
                            Bullet b = {PlayerShip.x+1, PlayerShip.y, 3, 5, 4, PlayerShip.damage, 1};
                            Bullets[i] = b;
                            break;
                        }
                    }
                    for (int i = 0; i < MAX_PROJECTILES; i++) {
                        if (Bullets[i].alive == 0) {
                            Bullet b = {PlayerShip.x + PlayerShip.width-5, PlayerShip.y, 3, 5, 4, PlayerShip.damage, 1};
                            Bullets[i] = b;
                            break;
                        }
                    }
                }

                // ENEMY SPAWNING 
                if (EnemiesAlive <= 0) {
                    EnemiesAlive = 0;
                    int attack = randint(0, 4);
                    if (attack == 0) {
                        int offset = randint(0, 10);
                        for (int i = 0; i < 3; i++) {
                            Enemy s = {1, -1*offset + 20 + i*20, -1*offset, 11, 9, 1, 1, 5};
                            Enemies[EnemiesAlive] = s;
                            EnemiesAlive += 1;
                        }
                        offset = randint(10, 30);
                        for (int i = 4; i < 8; i++) {
                            Enemy s = {1, offset+ 80 + i*20, -1*offset, 11, 9, 1, -1, 5};
                            Enemies[EnemiesAlive] = s;
                            EnemiesAlive += 1;
                        }
                        for (int i = 0; i < 3; i++) {
                            Enemy s = {1, 100+i*20, -50, 11, 9, 2, 0, 5};
                            Enemies[EnemiesAlive] = s;
                            EnemiesAlive += 1;
                        }
                    }
                    if (attack == 1) {
                        int rand = randint(40, 200);
                        for (int i = 0; i < 5; i++) {
                            Enemy a = {1, rand, 0 - i*20, 11, 9, 2, 0, 5};
                            Enemy b = {1, rand + 20, 0 - i*20, 11, 9, 2, 0, 5};
                            Enemies[EnemiesAlive] = a;
                            Enemies[EnemiesAlive+1] = b;
                            EnemiesAlive+=2;
                        }
                    }
                    if (attack == 3) {
                        int locd = randint(60,80);
                        for (int i = 0; i < 10; i++) {
                            int loc = randint(80, 180);
                            Enemy w = {1, loc, 0 - i*locd, 11, 9, 3, 0, 5};
                            Enemies[EnemiesAlive] = w;
                            EnemiesAlive += 1;
                        }
                    }
                } 
                lastAttack += 1; 

                //// DRAWING
                for (int i = 0; i < MAX_PROJECTILES; i++) {
                    if (Bullets[i].alive == 1) {
                        Bullet curr = Bullets[i];
                        drawPartialFull(curr.x-curr.speed, curr.y-2*curr.speed, 1*curr.width+2*curr.speed, 5*curr.height+2*curr.speed, gamebg);
                        drawRectDMA(curr.x, curr.y, curr.width, curr.height, RED);
                        Bullets[i].y -= curr.speed;

                        if (Bullets[i].y + Bullets[i].height <= 0) {
                            drawPartialFull(curr.x-curr.speed, curr.y-2*curr.speed, 1*curr.width+2*curr.speed, 5*curr.height+2*curr.speed, gamebg);
                            Bullets[i].alive = 0;
                        }
                    }
                }

                // ENEMY DRAWING + MOVEMENT
                for (int e = 0; e < MAX_ENEMIES; e++) {
                    Enemy curr = Enemies[e];
                    if ((curr.y >= 160 || (curr.x < 0 || curr.x + curr.width >= 240)) && Enemies[e].health > 0) {
                        Enemies[e].health = 0;
                        PlayerShip.health -= curr.damage;
                        EnemiesAlive -= 1;
                        drawPartialFull(curr.x-3, curr.y-3, curr.width+6, curr.height+6, gamebg);
                    }
                    if (Enemies[e].health > 0) {
                        drawPartialFull(curr.x-3, curr.y-3, curr.width+6, curr.height+6, gamebg);
                        drawImageDMA(curr.x, curr.y, curr.width, curr.height, evil_ship);
                        Enemies[e].y += curr.dy;
                        Enemies[e].x += curr.dx;
                    }

                    for (int i = 0; i < MAX_PROJECTILES; i++) {
                        if (Bullets[i].alive == 1 && curr.health > 0) {
                            if (Bullets[i].x >= curr.x && Bullets[i].x <= curr.x + curr.width) {
                                if (Bullets[i].y >= curr.y && Bullets[i].y <= curr.y + curr.height) {
                                    Enemies[e].health = 0;
                                    PlayerShip.score += 1;
                                    PlayerShip.health += 1;
                                    EnemiesAlive -= 1;
                                    Bullets[i].alive = 0;
                                    drawPartialFull(Bullets[i].x-Bullets[i].speed, Bullets[i].y-2*Bullets[i].speed, 1*Bullets[i].width+2*Bullets[i].speed, 5*Bullets[i].height+2*Bullets[i].speed, gamebg);
                                    drawPartialFull(curr.x-curr.dx, curr.y-2*curr.dy, curr.width+2*curr.dx, curr.height+2*curr.dy, gamebg);
                                }
                            }
                        }
                    }
                }

                if (PlayerShip.health <= 0) {
                    state = END;
                    drawFullScreenImageDMA(endgame);
                    break;
                }

                // PLAYER
                drawPartialFull(PlayerShip.x-PlayerShip.speed, PlayerShip.y-PlayerShip.speed, PlayerShip.width+2*PlayerShip.speed, PlayerShip.height+2*PlayerShip.speed, gamebg);
                drawImageDMA(PlayerShip.x, PlayerShip.y, PlayerShip.width, PlayerShip.height, player);

                // SCORE
                char buffer[128];
                drawPartialFull(160, 150, 100, 100, gamebg);
                drawPartialFull(2, 150, 70, 10, gamebg);
                snprintf(buffer,128,"HEALTH: %d", PlayerShip.health);
                drawString(160, 150, buffer, WHITE);
                snprintf(buffer,128,"SCORE: %d", PlayerShip.score);
                drawString(2, 150, buffer, WHITE);
                break;
            case END:
                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_A, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        break;
                    }
                }
                char bufed[128];
                snprintf(bufed,128,"SCORE: %d", PlayerShip.score);
                drawString(100, 70, bufed, WHITE);
                break;
        }
        previousButtons = currentButtons; 
    }
    return 0;
}