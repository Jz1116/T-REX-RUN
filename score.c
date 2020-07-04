#include "score.h"
#include <stdio.h>
#include <stdlib.h>

//count the number of digits a number has
int countNumberOfDigits(int num) {
    int count = 1;

    while (num >= 10) {
        count++;
        num = num / 10;
    }
    return count;
}

// get a specfic digit in a number
int getSpecificDigit(int num, int digit) {

    while (digit > 1) {
      num = num / 10;
      digit--;
    }

    return num % 10;
}

// populate all the digits of a number to an array
void populateArray(char *digits, int num_digits, int score) {
    for (int i = 0, j = num_digits; i < num_digits; i++) {
        digits[i] = getSpecificDigit(score, j) + '0';
        j--;
    }
    digits[num_digits] = '\0';
}
