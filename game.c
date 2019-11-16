#include "myLib.h"

typedef enum {
    START,
    INST,
    CONT,
    PLAY,
    END,
} GBAState;

const int MAX_ENEMIES = 30;
const int MAX_PROJECTILES = 15;

int main(void) {
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;
    GBAState state = START;
    Game GAMESTATE; 
    GAMESTATE.HIGH_SCORE = 0;

    while (1) {
        currentButtons = BUTTONS; // Load the current state of the buttons
        waitForVBlank();

        switch(state) {
            case START:
                drawFullScreenImageDMA(start_screen);
                resetGame(&GAMESTATE);
                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_A, currentButtons)) {
                        state = INST;
                        break;
                    }
                }
                if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        GAMESTATE.HIGH_SCORE = 0;
                        break;
                    }
                }
                break;
            case INST:
                drawFullScreenImageDMA(instructions);
                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_A, currentButtons)) {
                        state = CONT;
                        break;
                    }
                }
                if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        GAMESTATE.HIGH_SCORE = 0;
                        break;
                    }
                }
                break;
            case CONT:
                drawFullScreenImageDMA(controls);
                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_A, currentButtons)) {
                        state = PLAY;
                        drawFullScreenImageDMA(gamebg);
                        break;
                    }
                }
                if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        GAMESTATE.HIGH_SCORE = 0;
                        break;
                    }
                }
                break;
            case PLAY:
                if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        GAMESTATE.HIGH_SCORE = 0;
                        break;
                    }
                }

                // PLAYER MOVEMENT + ATTACK
                playerInput(&GAMESTATE, previousButtons, currentButtons);

                // ENEMY SPAWNING 
                spawnEnemies(&GAMESTATE);

                //// DRAWING
                drawGame(&GAMESTATE);

                if (GAMESTATE.PlayerShip.health <= 0) {
                    state = END;
                    drawFullScreenImageDMA(endgame);
                    break;
                }
                break;
            case END:
                drawScores(&GAMESTATE);
                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_A, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        break;
                    }
                }
                if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
                        state = START;
                        drawFullScreenImageDMA(start_screen);
                        GAMESTATE.HIGH_SCORE = 0;
                        break;
                    }
                }
        }
        previousButtons = currentButtons; 
    }
    return 0;
}