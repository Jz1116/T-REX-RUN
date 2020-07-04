#ifndef GAME_H
#define GAME_H

#include "gba.h"



                    /* TODO: */

            // Create any necessary structs //
// construct a trex_runner struct
typedef struct dinosaur {
    const u16 *image;
    int row;
    int col;
    int width;
    int height;
    int speed;
} Dinosaur;

// construct a cactus_big struct
typedef struct cactus_big {
    const u16 *image;
    int row;
    int col;
    int width;
    int height;
    int speed;
} Cactus_big;

// construct a cactus_small struct
typedef struct cactus_small {
    const u16 *image;
    int row;
    int col;
    int width;
    int height;
    int speed;
} Cactus_small;

typedef struct Bird {
    const u16 *image;
    int row;
    int col;
    int width;
    int height;
    int speed;
} Bird;

// construct obstacles struct that contains different types of obstacles
typedef struct obstacles {
    Cactus_small small_cactus;
    Cactus_big big_cactus;
    Bird bird;
    int type;
} Obstacles;

/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
*
*
*
*
* Example of a struct to hold state machine data:
*
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

#endif
