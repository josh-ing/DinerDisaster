#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/titlescreen.h"
#include "images/mainfood.h"
#include "images/projectile.h"
#include "images/customerdefault.h"
#include "images/mainguy.h"
#include "images/victory.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app.
#define PLAYER_WIDTH 35
#define PLAYER_HEIGHT 35
#define PROJ_WIDTH 25
#define PROJ_HEIGHT 25
#define CUSTOMER_WIDTH 35
#define CUSTOMER_HEIGHT 37


typedef enum {
  START,
  PLAY,
  WIN,
  LOSE,
} GBAState;

void unDraw(void) {
  //undraw player position
  drawRectDMA(cs.server.row, cs.server.col, PLAYER_WIDTH, PLAYER_HEIGHT, BLACK);
  //undraw over text
  drawRectDMA(0, 5, 160, 16, BLACK);
  //undraw over white rectangle
  drawRectDMA(55, 82, 75, 15, BLACK);
  //tomato projectiles
  for (int i = 0; i < cs.ntomato; i++) {
    drawRectDMA(cs.tomato[i].row, cs.tomato[i].col, 15, 15, BLACK);
  }
}




int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;

  int background = 0; //draw background once
  int a_down = 0;
  int previousA;
  cs.server.gameOver = 0;
  cs.custom.row = 8;
  cs.custom.col = 100;
  cs.server.score = 0;

  int deltas[] = {-3, -2, -1, 1, 2, 3};
  int ndeltas = sizeof(deltas) / sizeof(deltas[0]);

  //projectile starting state 
  for (int i = 0; i < 3; i++) {
    cs.tomato[i].row = randint(0, 160);
    cs.tomato[i].col = randint(0, 240);
    cs.tomato[i].rd = deltas[randint(0, ndeltas)];;
    cs.tomato[i].cd = deltas[randint(0, ndeltas)];;
  }
  cs.ntomato = 3;

  while (1) {
    currentButtons = BUTTONS;  // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    waitForVBlank();
    
    switch (state) {
      case START:

        cs.server.gameOver = 0;
        if (background == 0) {
          drawFullScreenImageDMA(titlescreen);
          drawString(150, 150, "Press start", RED);
          background = 1;
        } 
        

        previousA = a_down;
        a_down = currentButtons;
        if (KEY_DOWN(BUTTON_START, a_down) && !KEY_DOWN(BUTTON_START, previousA)) {
          fillScreenDMA(BLACK);
          int row1 = randint(0, 160);
          int col2 = randint(0, 240);
          for (int i = 0; i < cs.ntomato; i++) {
            if (row1 == (cs.tomato[i].row + 10) || 
                col2 == (cs.tomato[i].col + 10) ||
                row1 == (cs.tomato[i].row - 10) ||
                col2 == (cs.tomato[i].col - 10)) {
                  cs.server.row = randint(0, 160);
                  cs.server.col = randint(0, 240);
                }
          }
            cs.server.row = randint(0, 160);
            cs.server.col = randint(0, 240);
          cs.server.gameOver = 0;
          //draw the table you pick up food from
          drawRectDMA(55, 82, 75, 15, WHITE);
          drawImageDMA(8, 100, 35, 37, customerdefault);
          //draw score text
          char buffer[51];
          sprintf(buffer, "Score: %d", cs.server.score);
          drawString(0, 0, buffer, RED);
          state = PLAY;
        }
        break;
      case PLAY:
        waitForVBlank();
        unDraw();
        //button state
        previousA = a_down;
        a_down = currentButtons;

        //draw the table you pick up food from
        drawRectDMA(55, 82, 75, 15, WHITE);
        drawImageDMA(8, 100, 35, 37, customerdefault);
        //draw score text
        char buffer[51];
        sprintf(buffer, "Score: %d", cs.server.score);
        drawString(0, 0, buffer, RED);

        //draw projectiles
        for (int i = 0; i < cs.ntomato; i++) {
          cs.tomato[i].row = cs.tomato[i].row + (cs.tomato[i].rd / 2);
          cs.tomato[i].col += (cs.tomato[i].cd / 2);
          if (cs.tomato[i].row < 0) {
            cs.tomato[i].row = 0;
            cs.tomato[i].rd = -cs.tomato[i].rd;
          }
          if (cs.tomato[i].row > 159 - 25 + 1) {
            cs.tomato[i].row = 159 - 25 + 1;
            cs.tomato[i].rd = -cs.tomato[i].rd;
          }
          if (cs.tomato[i].col < 0) {
            cs.tomato[i].col = 0;
            cs.tomato[i].cd = -cs.tomato[i].cd;
          }
          if (cs.tomato[i].col > 239 - 25 + 1) {
            cs.tomato[i].col = 239 - 25 + 1;
            cs.tomato[i].cd = -cs.tomato[i].cd;
          }
        }


        if (KEY_DOWN(BUTTON_SELECT, a_down) && !KEY_DOWN(BUTTON_SELECT, previousA)) { //reset by pressing select during any state
          cs.server.gameOver = 0;
          cs.server.score = 0;
          background = 0;
          for (int i = 0; i < 3; i++) {
            cs.tomato[i].row = randint(0, 160);
            cs.tomato[i].col = randint(0, 240);
            cs.tomato[i].rd = deltas[randint(0, ndeltas)];;
            cs.tomato[i].cd = deltas[randint(0, ndeltas)];;
          }
          state = START;
        }

        if (KEY_DOWN(BUTTON_LEFT, a_down)) {
          cs.server.col = cs.server.col - 2;
          //draw rectangle with width of how much i moved (2) and height and make black. location = cs.server.col & cs.server.col
        } else if (KEY_DOWN(BUTTON_RIGHT, a_down)) {
          cs.server.col = cs.server.col + 2;
        }

        if (KEY_DOWN(BUTTON_UP, a_down)) {
          cs.server.row = cs.server.row - 2;
        } else if (KEY_DOWN(BUTTON_DOWN, a_down)) {
          cs.server.row = cs.server.row + 2;
        }

        //bound detection
        if (cs.server.row < 0) {
          cs.server.row = 0;
        }
        if (cs.server.row > 159 - PLAYER_WIDTH + 1) {
          cs.server.row = 159 - PLAYER_WIDTH + 1;
        }
        if (cs.server.col < 0) {
          cs.server.col = 0;
        }
        if (cs.server.col > 239 - PLAYER_WIDTH) {
          cs.server.col = 239 - PLAYER_WIDTH;
        }

        //customer collision
        if (cs.server.col < cs.custom.col + CUSTOMER_WIDTH &&
          cs.server.col + PLAYER_WIDTH > cs.custom.col && 
          cs.server.row < cs.custom.row + CUSTOMER_HEIGHT && 
          cs.server.row + PLAYER_HEIGHT > cs.custom.row) {
            cs.server.score = cs.server.score + 1;
            int row1 = randint(0, 160);
            int col2 = randint(0, 240);
            for (int i = 0; i < cs.ntomato; i++) {
              if (row1 == (cs.tomato[i].row + 10) || 
                  col2 == (cs.tomato[i].col + 10) ||
                  row1 == (cs.tomato[i].row - 10) ||
                  col2 == (cs.tomato[i].col - 10)) {
                    cs.server.row = randint(0, 160);
                    cs.server.col = randint(0, 240);
              }
            }
            cs.server.row = randint(0, 160);
            cs.server.col = randint(0, 240);
          
      }

      //projectile collision
      for (int i = 0; i < cs.ntomato; i++) {
        if (cs.server.col < cs.tomato[i].col + 5 &&
          cs.server.col + PLAYER_WIDTH > cs.tomato[i].col && 
          cs.server.row < cs.tomato[i].row + 5 && 
          cs.server.row + PLAYER_HEIGHT > cs.tomato[i].row) {
            cs.server.gameOver = 1;
          }
      }
      

        if (cs.server.score == 5) {
          background = 0;
          state = WIN;
        }

        if (cs.server.gameOver == 1) {
          background = 0;
          state = LOSE;
        }

        //redraw
        drawImageDMA(cs.server.row, cs.server.col, 35, 35, mainfood);
        
        for (int i = 0; i < cs.ntomato; i++) {
          drawImageDMA(cs.tomato[i].row, cs.tomato[i].col, 15, 15, projectile);
        }



        break;
      case WIN:

        previousA = a_down;
        a_down = currentButtons;
        drawFullScreenImageDMA(victory);
        cs.server.score = 0;
        cs.server.gameOver = 0;
        if (KEY_DOWN(BUTTON_SELECT, a_down) && !KEY_DOWN(BUTTON_SELECT, previousA)) { //reset by pressing select during any state
          background = 0;
          state = START;
          for (int i = 0; i < 3; i++) {
            cs.tomato[i].row = randint(0, 160);
            cs.tomato[i].col = randint(0, 240);
            cs.tomato[i].rd = deltas[randint(0, ndeltas)];;
            cs.tomato[i].cd = deltas[randint(0, ndeltas)];;
          }
        }

        break;
      case LOSE:

        previousA = a_down;
        a_down = currentButtons;
        fillScreenDMA(BLACK);
        drawString(80, 50, "Retry? Press select", RED);
        if (KEY_DOWN(BUTTON_SELECT, a_down) && !KEY_DOWN(BUTTON_SELECT, previousA)) { //reset by pressing select during any state
          background = 0;
          cs.server.gameOver = 0;
          cs.server.score = 0;
           for (int i = 0; i < 3; i++) {
            cs.tomato[i].row = randint(0, 160);
            cs.tomato[i].col = randint(0, 240);
            cs.tomato[i].rd = deltas[randint(0, ndeltas)];;
            cs.tomato[i].cd = deltas[randint(0, ndeltas)];;
           }
          state = START;
        }
        break;
    }

    waitForVBlank();

    previousButtons = currentButtons;  // Store the current state of the buttons
  }

   UNUSED(previousButtons);// You can remove this once previousButtons is used

  return 0;
}
