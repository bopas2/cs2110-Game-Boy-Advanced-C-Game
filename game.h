#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //

typedef struct player {
        int health;
        int x;
        int y;
        int width;
        int height;
        int speed;
        int score;
        int damage;
} Player;

typedef struct enemy {
        int health;
        int x;
        int y;
        int width;
        int height;
        int dy;
        int dx;
        int damage;
} Enemy;

typedef struct bullet {
        int x;
        int y;
        int width;
        int height;
        int speed;
        int damage;
        int alive; 
} Bullet;

#endif
