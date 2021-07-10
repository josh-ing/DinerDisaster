#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //

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

typedef struct player {
        int row;
        int col;
        int food;
        int gameOver;
        int score;

} Server;

typedef struct projectile {
        int row;
        int col;
        int width;
        int height;
        int rd;
        int cd;
} tomato;

//if press a, check not holding food
//if true, are they at counter (if row < 10 bc counter is at 10)

// next food is same type as vblank 
// if (vblank counter = next food)
//     spawn food
//     next food = vlank counter + food delay
typedef struct food {
        int row;
        int col;
        int width;
        int height;
} chicken;


typedef struct customer {
        int row; 
        int col;
        int width;
        int height;
        //Declare patience to be 20
        //on every frame, decrement patience
        //get mad at 0, leave
} custom;


struct state {
        custom custom;
        tomato tomato[3];
        Server server;
        int ntomato;

} cs, ps;




#endif
