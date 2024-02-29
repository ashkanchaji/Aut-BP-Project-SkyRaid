#include "menu.h"
#include "game.h"
#include "difficulty.h"
#include "color.h"

#include <stdio.h>
#include <unistd.h>
#include <conio.h>


int main(){
    // To hide the cursor
    printf("\e[?25l");

    // 1 is starting the game and 2 is show the records
    int retry = 0;
    short difficulty = 1;
    while (1){
        int command = showMenu();

        if (command == 1){
            while (!retry){
                retry = startGame(difficulty);
            }
        }
        else if (command == 2){
            difficulty = chooseDifficulty(difficulty);
        }
        // show the logs
        else if (command == 3){
            printf("%s", TC_GRN);
            FILE * fp = fopen("scores.txt", "r");
            if (fp == NULL){
                system("@cls||clear");
                printf("Could not open TXT file.\n");
                exit(-1);
            }
            int i = 1;
            while (!feof(fp)) {
                fseek(fp, -35 * i, SEEK_END);
                char buffer[35];
                char *record = fgets(buffer, 35, fp);
                if (record != NULL) {
                    printf("%c[%d;%df %s", 0x1B, i, 72, record);
                }
                i++;
            }
            fclose(fp);

            printf("\n%c[%d;%df %sTo return press \"Esc\" button.%s\n", 0x1B, i, 74, TC_RED, TC_NRM);

            char return_command;
            do {
                return_command = _getch();
            } while (return_command != 27);
        }
        system("@cls||clear");
        retry = 0;
    }
}
