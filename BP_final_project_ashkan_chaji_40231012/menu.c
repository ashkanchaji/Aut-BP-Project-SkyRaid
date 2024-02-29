#include "menu.h"
#include "color.h"

#include <stdio.h>
#include <unistd.h>
#include <conio.h>


int showMenu(void) {
    printf("%s", TC_GRN);
    printPlay();

    char command;
    int position = 1;

    do {
        command = _getch();
        system("@cls||clear");

        if (command == '\r'){
            printf("%s", TC_NRM);
            if (position != 4){
                return position;
            }
            else{
                printf("%c[%d;%df %sBye!%s\n", 0x1B, 26, 85, TC_B_CYN, TC_NRM);
                sleep(1);
                exit(-1);
            }
        }

        if (command == 80) {
            position++;
        } else if (command == 72) {
            position--;
        }

        // Ensure position stays within the valid range (1 to 4)
        if (position < 1) {
            position = 4;
        } else if (position > 4) {
            position = 1;
        }

        if (position == 1) {
            printPlay();
        } else if (position == 2) {
            printDifficulty();
        } else if (position == 3) {
            printRecords();
        } else {
            printExit();
        }
    } while (1);
}

void printPlay(){
    printf("%c[%d;%dfMove through the buttons using UP / Down arrows:\n\n", 0x1B, 23, 65);
    printf("%c[%d;%df %s>>%s\n", 0x1B, 26, 78, TC_RED, TC_GRN);
    printf("%c[%d;%df %sPlay%s\n", 0x1B, 26, 85, TC_RED, TC_GRN);
    printf("%c[%d;%df Difficulty\n", 0x1B, 27, 82);
    printf("%c[%d;%df Game records\n", 0x1B, 28, 81);
    printf("%c[%d;%df Exit\n", 0x1B, 29, 85);
}

void printDifficulty(){
    printf("%c[%d;%dfMove through the buttons using UP / Down arrows:\n\n", 0x1B, 23, 65);
    printf("%c[%d;%df Play\n", 0x1B, 26, 85);
    printf("%c[%d;%df %s>>%s\n", 0x1B, 27, 78, TC_RED, TC_GRN);
    printf("%c[%d;%df %sDifficulty%s\n", 0x1B, 27, 82, TC_RED, TC_GRN);
    printf("%c[%d;%df Game records\n", 0x1B, 28, 81);
    printf("%c[%d;%df Exit\n", 0x1B, 29, 85);
}

void printRecords(){
    printf("%c[%d;%dfMove through the buttons using UP / Down arrows:\n\n", 0x1B, 23, 65);
    printf("%c[%d;%df Play\n", 0x1B, 26, 85);
    printf("%c[%d;%df Difficulty\n", 0x1B, 27, 82);
    printf("%c[%d;%df %s>>%s\n", 0x1B, 28, 78, TC_RED, TC_GRN);
    printf("%c[%d;%df %sGame records%s\n", 0x1B, 28, 81, TC_RED, TC_GRN);
    printf("%c[%d;%df Exit\n", 0x1B, 29, 85);
}

void printExit(){
    printf("%c[%d;%dfMove through the buttons using UP / Down arrows:\n\n", 0x1B, 23, 65);
    printf("%c[%d;%df Play\n", 0x1B, 26, 85);
    printf("%c[%d;%df Difficulty\n", 0x1B, 27, 82);
    printf("%c[%d;%df Game records\n", 0x1B, 28, 81);
    printf("%c[%d;%df %s>>%s\n", 0x1B, 29, 78, TC_RED, TC_GRN);
    printf("%c[%d;%df %sExit%s\n", 0x1B, 29, 85, TC_RED, TC_GRN);
}