#define score_string_row 10
#define score_string_col 150
#define score_num_row 10
#define score_num_col 200
#define char_width 6
#define char_height 8




//count the number of digits a number has
int countNumberOfDigits(int num);

// get a specfic digit in a number
int getSpecificDigit(int num, int digit);

// populate the digits of a number to an array
void populateArray(char *string, int num_digits, int score);
