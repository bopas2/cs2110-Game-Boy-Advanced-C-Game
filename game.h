#ifndef GAME_H
#define GAME_H

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

typedef struct {
        int EnemiesAlive;
        Enemy Enemies[30];
        Bullet Bullets[15];
        Player PlayerShip;
        int MAX_PROJECTILES;
        int MAX_ENEMIES;
        int HIGH_SCORE;
} Game;

#endif
