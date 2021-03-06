#include "game.h"

#include <stdio.h>
#include <stdlib.h>

/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"
#include "images/start_screen.h"
#include "images/garbage.h"
#include "images/runner.h"
#include "images/cactus_big.h"
#include "images/cactus_small.h"
#include "images/lose_screen.h"
#include "score.h"
#include "utility.h"
#include "images/bird.h"

/* TODO: */
// Add any additional states you need for your app.
typedef enum {
  START,
  PLAY,
  WIN,
  LOSE,
} GBAState;

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;
  int score = 0;             // initialize score
  int obstacle_speed = 1;      // initialze obstacle speeds
  int runner_speed = 6;       // initialize runner speed
  // create the trex_runner
  Dinosaur trex_runner = {runner, 88, 15, RUNNER_WIDTH, RUNNER_HEIGHT, runner_speed};
  // create a big cactus
  Cactus_big big_cactus = {cactus_big, 86, 240 - CACTUS_BIG_WIDTH, CACTUS_BIG_WIDTH, CACTUS_BIG_HEIGHT, obstacle_speed};
  // create a small cactus
  Cactus_small small_cactus = {cactus_small, 93, 240 - CACTUS_SMALL_WIDTH, CACTUS_SMALL_WIDTH, CACTUS_SMALL_HEIGHT, obstacle_speed};
  // create a Bird
  Bird bird = {bird_image, 50, 240 - BIRD_WIDTH, BIRD_WIDTH, BIRD_HEIGHT, obstacle_speed};
  // incorporate
  Obstacles obstacle = {small_cactus, big_cactus, bird, 3};

  while (1) {
    currentButtons = BUTTONS;  // Load the current state of the buttons



    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    switch (state) {
      case START:
        // get the start screen
        waitForVBlank();
        drawCenteredString(130, 55, 200, 10, "PRESS <START>", BLACK);
        waitForVBlank();
        drawFullScreenImageDMA(start_screen);

        // if the user presses START, then the state turns into PLAY
        if KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons) {
          waitForVBlank();
          // fill the full screen with white
          fillScreenDMA(WHITE);
          waitForVBlank();
          drawRectDMA(110, 0, WIDTH, 1, GRAY);   // draw a horizontal line on the screen
          waitForVBlank();
          //draw the trex_runner
          drawImageDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);
          waitForVBlank();
          // display the 'Score: ' string
          drawString(score_string_row, score_string_col, "Score: ", BLACK);
          // display 0 besides 'Score: '
          drawChar(score_num_row, score_num_col, score + '0', BLACK);
          state = PLAY;
        }

        break;
      case PLAY:
        drawCenteredString(130, 0, 100, 10, "PRESS <SELECT>", BLACK);  // display the instruction
        drawCenteredString(140, 0, 100, 10, "TO GO HOME", BLACK);
        drawCenteredString(130, 95, 200, 10, "PRESS <UP>", BLACK);     // display the instruction
        drawCenteredString(140, 95, 200, 10, "TO JUMP", BLACK);
        // if there is no overlap, detectedCollision stays at 0
        // if there is an overlap, detectCollision becomes 1
        int detectedCollision = 0;

        while (1) {

          // initialize properties for the obstacle
          int obstacle_col = obstacle.small_cactus.row;
          int obstacle_row = obstacle.small_cactus.col;
          int obstacle_width = obstacle.small_cactus.width;
          int obstacle_height = obstacle.small_cactus.height;
          const u16 *obstacle_image = obstacle.small_cactus.image;

          // get a random number
          int number = randint(0, 3);
          // if the number is 0, then the obstacle is a small cactus
          if (number == 0) {
              obstacle_row = obstacle.small_cactus.row;
              obstacle_col = obstacle.small_cactus.col;
              obstacle_width = obstacle.small_cactus.width;
              obstacle_height = obstacle.small_cactus.height;
              obstacle_image = obstacle.small_cactus.image;
          } else if (number == 1) {   // if the number is 1, then it is a big cactus
              obstacle_row = obstacle.big_cactus.row;
              obstacle_col = obstacle.big_cactus.col;
              obstacle_width = obstacle.big_cactus.width;
              obstacle_height = obstacle.big_cactus.height;
              obstacle_image = obstacle.big_cactus.image;
          } else if (number == 2) { // if the number is 2, then it is a bird
            obstacle_row = obstacle.bird.row;
            obstacle_col = obstacle.bird.col;
            obstacle_width = obstacle.bird.width;
            obstacle_height = obstacle.bird.height;
            obstacle_image = obstacle.bird.image;
          }



          // create the motion for obstacles
          for (int k = obstacle_col; k >= 0; k -= obstacle_speed) {
            score++;   // increment the score
            int num_digits = countNumberOfDigits(score); // count digits
            char digits[num_digits + 1];                 // create an array to store each digit of a score
            populateArray(digits, num_digits, score);
            drawString(score_num_row, score_num_col, digits, BLACK);    // display the score

            // display the obstacle
            drawImageDMA(obstacle_row, k, obstacle_width, obstacle_height, obstacle_image);
            waitForVBlank();

            // cover the previous score
            drawRectDMA(score_num_row, score_num_col, (2 * num_digits - 1) * char_width, char_height, WHITE);
            // cover the previous obstacle position
            drawRectDMA(obstacle_row, k, obstacle_width, obstacle_height, WHITE);

            // function to detect collision
            detectedCollision = detectCollision(trex_runner.row,
                                                trex_runner.col,
                                                trex_runner.width,
                                                trex_runner.height,
                                                obstacle_row,
                                                k,
                                                obstacle_width,
                                                obstacle_height);

            // if it detects collision, change state to LOSE, and break this for loop
            if (detectedCollision) {
                state = LOSE;
                break;
            }

            // if the user presses SELECT, then go to the launching page
            if KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons) {
              state = START;
              score = 0;                // reassign the score to 0
              obstacle_speed = 1;       // reassign obstacle_speed to 1
              trex_runner.row = 88;     // reassign trex_runner row
              trex_runner.col = 15;     // reassign trex_runner col
              break;
            }

            // if the user presses UP, then trex_runner will jump up
            // and then come back to the original position
            if KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons) {
                // wipe out the previous trex_runner position
                drawRectDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, WHITE);

                // store the k as obs_col so when runner jumps, the obstacle also moves
                int obs_col = k;

                // trex_runner jumps up
                for (int i = 1; i <= 50; i++) {
                  // ensure the score also moves as trex_runner jumps
                  score++;
                  int num_digits = countNumberOfDigits(score);
                  char digits[num_digits + 1];
                  populateArray(digits, num_digits, score);

                  // display trex_runner current position
                  drawImageDMA(trex_runner.row - i, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);
                  // display current score
                  drawString(score_num_row, score_num_col, digits, BLACK);
                  // display current obstacle position if it does not falls out of screen
                  if (obs_col >= 0) {
                    drawImageDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, obstacle_image);
                  }

                  waitForVBlank();

                  // wipe out trex_runner previous position
                  drawRectDMA(trex_runner.row - i, trex_runner.col, trex_runner.width, trex_runner.height, WHITE);
                  // wipe out previous score
                  drawRectDMA(score_num_row, score_num_col, (2 * num_digits - 1) * char_width, char_height, WHITE);
                  // wipe out previous obstacle position if it does not falls out of screen
                  if (obs_col >= 0) {
                    drawRectDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, WHITE);
                  }
                  // as trex_runner moves, to detect if there is a collision
                  detectedCollision = detectCollision(trex_runner.row - i,
                                                      trex_runner.col,
                                                      trex_runner.width,
                                                      trex_runner.height,
                                                      obstacle_row,
                                                      obs_col,
                                                      obstacle_width,
                                                      obstacle_height);

                  // if it detects collision, change state to LOSE, and break this for loop
                  if (detectedCollision) {
                      state = LOSE;
                      break;
                  }
                  // move the obstacle towards the left
                  obs_col -= obstacle_speed;
                }

                // if it detects Collision, then break this for loop
                if (detectedCollision) {
                    break;
                }

                // trex_runner jumps down
                for (int i = 1; i <= 50; i++) {
                  // ensure the score also moves as trex_runner jumps
                  score++;
                  int num_digits = countNumberOfDigits(score);
                  char digits[num_digits + 1];
                  populateArray(digits, num_digits, score);

                  // display the current trex_runner position
                  drawImageDMA(trex_runner.row - 50 + i, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);
                  // display the current score
                  drawString(score_num_row, score_num_col, digits, BLACK);
                  // display current obstacle position if it does not falls out of screen
                  if (obs_col >= 0) {
                    drawImageDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, obstacle_image);
                  }

                  waitForVBlank();

                  // wipe out the previous trex_runner position
                  drawRectDMA(trex_runner.row - 50 + i, trex_runner.col, trex_runner.width, trex_runner.height, WHITE);
                  // wipe out the previous score value
                  drawRectDMA(score_num_row, score_num_col, (2 * num_digits - 1) * char_width, char_height, WHITE);
                  // wipe out the previous obstacle position if it does not fall out of screen
                  if (obs_col >= 0) {
                    drawRectDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, WHITE);
                  }

                  // as the trex_runner jumps down, to detect if there is a collision
                  detectedCollision = detectCollision(trex_runner.row - 50 +  i,
                                                      trex_runner.col,
                                                      trex_runner.width,
                                                      trex_runner.height,
                                                      obstacle_row,
                                                      obs_col,
                                                      obstacle_width,
                                                      obstacle_height);

                  // if there is a collision, then change state to LOSE and break this for loop
                  if (detectedCollision) {
                      state = LOSE;
                      break;
                  }
                  // move the obstacle to the left
                  obs_col -= obstacle_speed;
                }

                // display the trex_runner in its original position
                drawImageDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);
                // assign obs_col value to k
                k = obs_col;
            }

            // if the user presses RIGHT, the trex_runner goes in the right direction
            // not normaly used
            if KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons) {
              // wipe out the previous trex_runner position
              drawRectDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, WHITE);

              // store the k as obs_col so when runner jumps, the obstacle also moves
              int obs_col = k;

              // ensure the score also moves as trex_runner jumps
              score++;
              int num_digits = countNumberOfDigits(score);
              char digits[num_digits + 1];
              populateArray(digits, num_digits, score);

              // trex_runner runs to the right
              // check if trex_runner falls out of the screen
              if (trex_runner.col + trex_runner.width + trex_runner.speed < WIDTH) {
                  trex_runner.col += trex_runner.speed;
              } else {
                  trex_runner.col = WIDTH - trex_runner.width;
              }

              // display trex_runner current position
                drawImageDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);

              // display current score
              drawString(score_num_row, score_num_col, digits, BLACK);

              // display current obstacle position if it does not falls out of screen
              if (obs_col >= 0) {
                drawImageDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, obstacle_image);
              }

              waitForVBlank();

              // wipe out trex_runner previous position
              drawRectDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, WHITE);

              // wipe out previous score
              drawRectDMA(score_num_row, score_num_col, (2 * num_digits - 1) * char_width, char_height, WHITE);
              // wipe out previous obstacle position if it does not falls out of screen
              if (obs_col >= 0) {
                drawRectDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, WHITE);
              }
              // as trex_runner moves, to detect if there is a collision
              detectedCollision = detectCollision(trex_runner.row,
                                                  trex_runner.col,
                                                  trex_runner.width,
                                                  trex_runner.height,
                                                  obstacle_row,
                                                  obs_col,
                                                  obstacle_width,
                                                  obstacle_height);

                // if it detects collision, change state to LOSE, and break this for loop
                if (detectedCollision) {
                    state = LOSE;
                }
                // move the obstacle towards the left
                obs_col -= obstacle_speed;

              // display the trex_runner in its position
              drawImageDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);
              // assign obs_col value to k
              k = obs_col;
            }

            if (detectedCollision) {
                break;
            }

            // if the user presses LEFT, the trex_runner goes in the left direction
            // not normaly used
            if KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons) {
              // wipe out the previous trex_runner position
              drawRectDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, WHITE);

              // store the k as obs_col so when runner jumps, the obstacle also moves
              int obs_col = k;

              // ensure the score also moves as trex_runner jumps
              score++;
              int num_digits = countNumberOfDigits(score);
              char digits[num_digits + 1];
              populateArray(digits, num_digits, score);

              // trex_runner runs to the left
              if (trex_runner.col - trex_runner.speed >= 0) {
                  trex_runner.col -= trex_runner.speed;
              } else {
                  trex_runner.col = 0; // ensure that trex_runner will not fall out of the border
              }

              // display trex_runner current position
              drawImageDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);
              // display current score
              drawString(score_num_row, score_num_col, digits, BLACK);
              // display current obstacle position if it does not falls out of screen
              if (obs_col >= 0) {
                drawImageDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, obstacle_image);
              }

              waitForVBlank();

              // wipe out trex_runner previous position
              drawRectDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, WHITE);

              // wipe out previous score
              drawRectDMA(score_num_row, score_num_col, (2 * num_digits - 1) * char_width, char_height, WHITE);
              // wipe out previous obstacle position if it does not falls out of screen
              if (obs_col >= 0) {
                drawRectDMA(obstacle_row, obs_col, obstacle_width, obstacle_height, WHITE);
              }
              // as trex_runner moves, to detect if there is a collision
              detectedCollision = detectCollision(trex_runner.row,
                                                  trex_runner.col,
                                                  trex_runner.width,
                                                  trex_runner.height,
                                                  obstacle_row,
                                                  obs_col,
                                                  obstacle_width,
                                                  obstacle_height);

                // if it detects collision, change state to LOSE, and break this for loop
                if (detectedCollision) {
                    state = LOSE;
                }
                // move the obstacle towards the left
                obs_col -= obstacle_speed;

              // display the trex_runner in its position
              drawImageDMA(trex_runner.row, trex_runner.col, trex_runner.width, trex_runner.height, trex_runner.image);
              // assign obs_col value to k
              k = obs_col;
            }


            previousButtons = currentButtons; // store current state of BUTTONS
            currentButtons = BUTTONS;         // load the current state
          }

          // if there is a collision or the state is START, break this while loop
          if (detectedCollision || state == START) {
              break;
          }

          // if the score is above 4000, change the speed to 4
          // if the score is above 2000, change the speed to 3
          // if the score is above 1000, change the speed to 2
          if (score >= 4000) {
              obstacle_speed = 4;
          } else if (score >= 2000) {
              obstacle_speed = 3;
          } else if (score >= 1000) {
              obstacle_speed = 2;
          }

        }

        break;
      case WIN:

        // there is no WIN condition for this game
        break;
      case LOSE:
        // keep the score not moving
        score++;
        score--;
        int num_digits = countNumberOfDigits(score);
        char digits[num_digits + 1];
        populateArray(digits, num_digits, score);

        // display Score and its score value
        drawString(100, 150, "Score: ", BLACK);
        drawString(100, 190, digits, BLACK);

        // display necessary text to retry this game
        drawString(115, 150, "PRESS <SELECT>", BLACK);
        drawString(125, 150, "RETRY", BLACK);
        waitForVBlank();
        // fill the screen with lose_screen
        drawFullScreenImageDMA(lose_screen);


        // if the user presses SELECT, then the game is restarted
        if KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons) {
            // state becomes START
            state = START;
            score = 0;                // reassign the score to 0
            obstacle_speed = 1;       // reassign obstacle_speed to 1
            trex_runner.row = 88;     // reassign trex_runner row
            trex_runner.col = 15;     // reassign trex_runner col
        }


        break;
    }

    previousButtons = currentButtons;  // Store the current state of the buttons
  }

  return 0;
}
