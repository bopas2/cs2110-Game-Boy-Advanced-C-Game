#include "myLib.h"

void drawProjectiles(Game *GAMESTATE) {
    extern int MAX_PROJECTILES;
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (GAMESTATE->Bullets[i].alive == 1) {
            Bullet curr = GAMESTATE->Bullets[i];
            drawPartialFull(curr.x-curr.speed, curr.y-2*curr.speed, 1*curr.width+2*curr.speed, 5*curr.height+2*curr.speed, gamebg);
            drawRectDMA(curr.x, curr.y, curr.width, curr.height, RED);
            GAMESTATE->Bullets[i].y -= curr.speed;

            if (GAMESTATE->Bullets[i].y + GAMESTATE->Bullets[i].height <= 0) {
                drawPartialFull(curr.x-curr.speed, curr.y-2*curr.speed, 1*curr.width+2*curr.speed, 5*curr.height+2*curr.speed, gamebg);
                GAMESTATE->Bullets[i].alive = 0;
            }
        }
    }
}

void resetGame(Game *GAMESTATE) {
    GAMESTATE->PlayerShip.health = 100;
    GAMESTATE->PlayerShip.x = 120;
    GAMESTATE->PlayerShip.y = 80;
    GAMESTATE->PlayerShip.width = 17;
    GAMESTATE->PlayerShip.height = 17;
    GAMESTATE->PlayerShip.speed = 3;
    GAMESTATE->PlayerShip.score = 0;
    GAMESTATE->PlayerShip.damage = 10;
    GAMESTATE->EnemiesAlive = 0;
    for (int i = 0; i < GAMESTATE->MAX_PROJECTILES; i++) {
        Bullet s = {0, 0, 0, 0, 0, 0, 0};
        GAMESTATE->Bullets[i] = s;
    }
    for (int e = 0; e < GAMESTATE->MAX_ENEMIES; e++) {
        GAMESTATE->Enemies[e].health = 0;
        GAMESTATE->Enemies[e].y = -10;
    }
}

void drawPlayer(Game *GAMESTATE) {
    drawPartialFull(GAMESTATE->PlayerShip.x-GAMESTATE->PlayerShip.speed, GAMESTATE->PlayerShip.y-GAMESTATE->PlayerShip.speed, GAMESTATE->PlayerShip.width+2*GAMESTATE->PlayerShip.speed, GAMESTATE->PlayerShip.height+2*GAMESTATE->PlayerShip.speed, gamebg);
    drawImageDMA(GAMESTATE->PlayerShip.x, GAMESTATE->PlayerShip.y, GAMESTATE->PlayerShip.width, GAMESTATE->PlayerShip.height, player);
}

void drawText(Game *GAMESTATE) {
    char buffer[128];
    drawPartialFull(160, 150, 100, 100, gamebg);
    drawPartialFull(2, 150, 70, 10, gamebg);
    snprintf(buffer,128,"HEALTH: %d", GAMESTATE->PlayerShip.health);
    drawString(160, 150, buffer, WHITE);
    snprintf(buffer,128,"SCORE: %d", GAMESTATE->PlayerShip.score);
    drawString(2, 150, buffer, WHITE);
}

void drawGame(Game *GAMESTATE) {
    drawProjectiles(GAMESTATE);
    drawEnemies(GAMESTATE);
    drawPlayer(GAMESTATE);
    drawText(GAMESTATE);
}

void drawEnemies(Game *GAMESTATE) {
    extern int MAX_PROJECTILES;
    extern int MAX_ENEMIES;
    for (int e = 0; e < MAX_ENEMIES; e++) {
        Enemy curr = GAMESTATE->Enemies[e];
        if ((curr.y >= 160 || (curr.x < 0 || curr.x + curr.width >= 240)) && GAMESTATE->Enemies[e].health > 0) {
            GAMESTATE->Enemies[e].health = 0;
            GAMESTATE->PlayerShip.health -= curr.damage;
            GAMESTATE->EnemiesAlive -= 1;
            drawPartialFull(curr.x-3, curr.y-3, curr.width+6, curr.height+6, gamebg);
        }
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (GAMESTATE->Bullets[i].alive == 1 && curr.health > 0) {
                if (GAMESTATE->Bullets[i].x >= curr.x && GAMESTATE->Bullets[i].x <= curr.x + curr.width) {
                    if (GAMESTATE->Bullets[i].y >= curr.y && GAMESTATE->Bullets[i].y <= curr.y + curr.height) {
                        GAMESTATE->Enemies[e].health = 0;
                        GAMESTATE->PlayerShip.score += 1;
                        GAMESTATE->PlayerShip.health += 1;
                        GAMESTATE->EnemiesAlive -= 1;
                        GAMESTATE->Bullets[i].alive = 0;
                        drawPartialFull(GAMESTATE->Bullets[i].x-GAMESTATE->Bullets[i].speed, GAMESTATE->Bullets[i].y-2*GAMESTATE->Bullets[i].speed, 1*GAMESTATE->Bullets[i].width+2*GAMESTATE->Bullets[i].speed, 5*GAMESTATE->Bullets[i].height+2*GAMESTATE->Bullets[i].speed, gamebg);
                        drawPartialFull(curr.x-3, curr.y-3, curr.width+6, curr.height+6, gamebg);
                    }
                }
            }
        }
        if (GAMESTATE->Enemies[e].health > 0) {
            drawPartialFull(curr.x-3, curr.y-3, curr.width+6, curr.height+6, gamebg);
            drawImageDMA(curr.x, curr.y, curr.width, curr.height, evil_ship);
            GAMESTATE->Enemies[e].y += curr.dy;
            GAMESTATE->Enemies[e].x += curr.dx;
        }
    }
}

void drawScores(Game *GAMESTATE) {
    if (GAMESTATE->PlayerShip.score >= GAMESTATE->HIGH_SCORE) {
        GAMESTATE->HIGH_SCORE = GAMESTATE->PlayerShip.score;
    }
    char bufed[128];
    snprintf(bufed,128,"SCORE: %d", GAMESTATE->PlayerShip.score);
    drawString(95, 50, bufed, WHITE);
    snprintf(bufed,128,"HIGHSCORE: %d", GAMESTATE->HIGH_SCORE);
    drawString(90, 70, bufed, WHITE);
}

void spawnEnemies(Game *GAMESTATE) {
    extern int MAX_ENEMIES;
    if (GAMESTATE->EnemiesAlive <= 0) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            Enemy curr = GAMESTATE->Enemies[i];
            GAMESTATE->Enemies[i].health = 0;
            drawPartialFull(curr.x-3, curr.y-3, curr.width+6, curr.height+6, gamebg);
        }
        GAMESTATE->EnemiesAlive = 0;
        int attack = randint(0, 4);
        if (attack == 0) {
            int offset = randint(0, 10);
            for (int i = 0; i < 3; i++) {
                Enemy s = {1, -1*offset + 20 + i*20, -1*offset, 11, 9, 1, 1, 5};
                GAMESTATE->Enemies[GAMESTATE->EnemiesAlive] = s;
                GAMESTATE->EnemiesAlive += 1;
            }
            offset = randint(10, 30);
            for (int i = 4; i < 8; i++) {
                Enemy s = {1, offset+ 80 + i*20, -1*offset, 11, 9, 1, -1, 5};
                GAMESTATE->Enemies[GAMESTATE->EnemiesAlive] = s;
                GAMESTATE->EnemiesAlive += 1;
            }
            for (int i = 0; i < 3; i++) {
                Enemy s = {1, 100+i*20, -50, 11, 9, 2, 0, 5};
                GAMESTATE->Enemies[GAMESTATE->EnemiesAlive] = s;
                GAMESTATE->EnemiesAlive += 1;
            }
        }
        if (attack == 1) {
            int rand = randint(40, 200);
            for (int i = 0; i < 5; i++) {
                Enemy a = {1, rand, 0 - i*40, 11, 9, 2, 0, 5};
                Enemy b = {1, rand + 20, 0 - i*40, 11, 9, 2, 0, 5};
                GAMESTATE->Enemies[GAMESTATE->EnemiesAlive] = a;
                GAMESTATE->Enemies[GAMESTATE->EnemiesAlive+1] = b;
                GAMESTATE->EnemiesAlive+=2;
            }
        }
        if (attack == 3 || attack == 4) {
            int locd = randint(40,80);
            for (int i = 0; i < 10; i++) {
                int loc = randint(80, 180);
                Enemy w = {1, loc, 0 - i*locd, 11, 9, 3, 0, 5};
                GAMESTATE->Enemies[GAMESTATE->EnemiesAlive] = w;
                GAMESTATE->EnemiesAlive += 1;
            }
        }
    }
}

void playerInput(Game *GAMESTATE, u32 previousButtons, u32 currentButtons) {
    drawImageDMA(GAMESTATE->PlayerShip.x, GAMESTATE->PlayerShip.y, GAMESTATE->PlayerShip.width, GAMESTATE->PlayerShip.height, player);
    if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
        if (GAMESTATE->PlayerShip.x + GAMESTATE->PlayerShip.speed + 2*GAMESTATE->PlayerShip.width < 260) {
            GAMESTATE->PlayerShip.x += GAMESTATE->PlayerShip.speed;
        }
    }
    if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
        if (GAMESTATE->PlayerShip.x - GAMESTATE->PlayerShip.speed >= 0) {
            GAMESTATE->PlayerShip.x -= GAMESTATE->PlayerShip.speed;
        }
    }
    if (KEY_DOWN(BUTTON_UP, currentButtons)) {
        if (GAMESTATE->PlayerShip.y - GAMESTATE->PlayerShip.speed >= 0) {
            GAMESTATE->PlayerShip.y -= GAMESTATE->PlayerShip.speed;
        }
    }
    if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
        if (GAMESTATE->PlayerShip.y + GAMESTATE->PlayerShip.speed + GAMESTATE->PlayerShip.height < 160) {
            GAMESTATE->PlayerShip.y += GAMESTATE->PlayerShip.speed;
        }
    }
    if (KEY_JUST_PRESSED(BUTTON_B, currentButtons, previousButtons)) {
        for (int i = 0; i < GAMESTATE->MAX_PROJECTILES; i++) {
            if (GAMESTATE->Bullets[i].alive == 0) {
                Bullet b = {GAMESTATE->PlayerShip.x+1, GAMESTATE->PlayerShip.y, 3, 5, 4, GAMESTATE->PlayerShip.damage, 1};
                GAMESTATE->Bullets[i] = b;
                break;
            }
        }
        for (int i = 0; i < GAMESTATE->MAX_PROJECTILES; i++) {
            if (GAMESTATE->Bullets[i].alive == 0) {
                Bullet b = {GAMESTATE->PlayerShip.x + GAMESTATE->PlayerShip.width-5, GAMESTATE->PlayerShip.y, 3, 5, 4, GAMESTATE->PlayerShip.damage, 1};
                GAMESTATE->Bullets[i] = b;
                break;
            }
        }
    }
}