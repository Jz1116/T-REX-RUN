#include "utility.h"
#include "gba.h"
#include "game.h"

// the function used to detect collision between the runner and obstacle
int detectCollision(int trex_runner_row,
                    int trex_runner_col,
                    int trex_runner_width,
                    int trex_runner_height,
                    int obstacle_row,
                    int obstacle_col,
                    int obstacle_width,
                    int obstacle_height) {

    // get trex_runner top-left corner's x position and y position
    int runner_tl_x = trex_runner_col;
    int runner_tl_y = trex_runner_row;

    // get trex_runner bottom-right corner's x position and y position
    int runner_br_x = trex_runner_col + trex_runner_width;
    int runner_br_y = trex_runner_row + trex_runner_height;

    // get obstacle top-left corner's x position and y position
    int obs_tl_x = obstacle_col;
    int obs_tl_y = obstacle_row;

    // get obstacle bottom-right corner's x position and y position
    int obs_br_x = obstacle_col + obstacle_width;
    int obs_br_y = obstacle_row + obstacle_height;

    // do comparision
    // if it is 1, then it detects collision
    // if it is 0, then it doesn't detect collision
    if (!(runner_br_x <= obs_tl_x || runner_br_y <= obs_tl_y
          || runner_tl_x >= obs_br_x || runner_tl_y >= obs_br_y)) {
        return 1;
    } else {
        return 0;
    }
}
