#include "difficulty.h"
#include "color.h"

#include <stdio.h>
#include <conio.h>

short chooseDifficulty(short difficulty){
    printf("%s", TC_GRN);

    char command;
    short position = difficulty;

    while (1){
        if (position == 0) {
            showEasy();
        } else if (position == 1) {
            showNormal();
        } else{
            showHard();
        }

        command = _getch();
        system("@cls||clear");

        if (command == '\r'){
            printf("%s", TC_NRM);
            return position;
        }

        if (command == 80) {
            position++;
        } else if (command == 72) {
            position--;
        }

        // Ensure position stays within the valid range (1 to 3)
        if (position < 0) {
            position = 2;
        } else if (position > 2) {
            position = 0;
        }
    }
}

void showEasy(){
    printf("%c[%d;%df Choose your difficulty :\n", 0x1B, 23, 75);
    printf("%c[%d;%df %s>>%s\n", 0x1B, 26, 81, TC_RED, TC_GRN);
    printf("%c[%d;%df %sEasy%s\n", 0x1B, 26, 85, TC_RED, TC_GRN);
    printf("%c[%d;%df Normal\n", 0x1B, 27, 84);
    printf("%c[%d;%df Hard\n", 0x1B, 28, 85);
}

void showNormal(){
    printf("%c[%d;%df Choose your difficulty :\n", 0x1B, 23, 75);
    printf("%c[%d;%df Easy\n", 0x1B, 26, 85);
    printf("%c[%d;%df %s>>%s\n", 0x1B, 27, 81, TC_RED, TC_GRN);
    printf("%c[%d;%df %sNormal%s\n", 0x1B, 27, 84, TC_RED, TC_GRN);
    printf("%c[%d;%df Hard\n", 0x1B, 28, 85);
}

void showHard(){
    printf("%c[%d;%df Choose your difficulty :\n", 0x1B, 23, 75);
    printf("%c[%d;%df Easy\n", 0x1B, 26, 85);
    printf("%c[%d;%df Normal\n", 0x1B, 27, 84);
    printf("%c[%d;%df %s>>%s\n", 0x1B, 28, 81, TC_RED, TC_GRN);
    printf("%c[%d;%df %sHard%s\n", 0x1B, 28, 85, TC_RED, TC_GRN);
}