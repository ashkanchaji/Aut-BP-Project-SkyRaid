#include "board.h"
#include "game.h"
#include "color.h"

#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <windows.h>


typedef struct {
    short mine;
    short cargoShip;
    short fighter;
    short bomber;
    short drone;
} spawnChance;

spawnChance level[3] = {{17, 21, 23, 29, 41},
                        {11, 17, 19, 21,37},
                        {7, 11, 13, 17, 31}};



int startGame(int difficulty) {
    createBoard();
    int position = 0;
    int center = (WIDTH / 2);
    int y_position = HEIGHT - 6;

    int score = 0;
    time_t start_time = time(NULL);
    time_t base_time = time(NULL);

    int i = 0, k = 1, m = 2, n = 3, o = 4;
    int j = 0;

    bool key_100 = false;
    bool key_250 = false;
    bool key_500 = false;
    bool key_1000 = false;

    bool EOG = false;
    while(!EOG){
        char command;
        if (kbhit()){
            command = _getch();
            if ((command == 75 || tolower(command) == 97) && center > 6){
                position -= 7;
            }
            else if ((command == 77 || tolower(command) == 100) && center < 146){
                position += 7;
            }
            else if ((command == 72 || tolower(command) == 119) && y_position > 3){
                y_position -= 3;
            }
            else if ((command == 80 || tolower(command) == 115) && y_position < HEIGHT - 6){
                y_position += 3;
            }
            else if (command == 32){
                if (j < 4){
                    fireBullets(center);
                    j++;
                }
                else {
                    j = 0;
                }
            }
        }

        if (score >= 100 && key_100 == false){
            spawnChance newDiff[3] = {{13, 17, 21, 27, 37},
                                       {9, 13, 17, 19, 34},
                                       {6, 11, 11, 17, 29}};
            level[0] = newDiff[0];
            level[1] = newDiff[1];
            level[2] = newDiff[2];
            key_100 = true;
        } else if (score >= 250 && key_250 == false){
            spawnChance newDiff[3] = {{11, 13, 17, 21, 34},
                                       {7, 11, 13, 17, 31},
                                       {5, 7, 9, 13, 27}};
            level[0] = newDiff[0];
            level[1] = newDiff[1];
            level[2] = newDiff[2];
            key_250 = true;
        } else if (score >= 500 && key_500 == false){
            spawnChance newDiff[3] = {{9, 11, 13, 19, 31},
                                      {6, 9, 11, 13, 29},
                                      {4, 5, 7, 11, 23}};
            level[0] = newDiff[0];
            level[1] = newDiff[1];
            level[2] = newDiff[2];
            key_500 = true;
        } else if (score >= 1000 && key_1000 == false){
            spawnChance newDiff[3] = {{7, 9, 11, 17, 29},
                                      {5, 7, 9, 11, 23},
                                      {3, 4, 5, 7, 19}};
            level[0] = newDiff[0];
            level[1] = newDiff[1];
            level[2] = newDiff[2];
            key_1000 = true;
        }

        if (i > 4){
            i = 0;
        }
        if (k > 6){
            k = 0;
        }
        if (n > 8){
            n = 0;
        }
        if (m > 12){
            m = 0;
        }
        if (o > 17){
            o = 0;
        }
        srand((unsigned)time(NULL));
        unsigned seed = (unsigned)time(NULL) + i;
        srand(seed);

        int mineSpawnChance = rand() % level[difficulty].mine;
        if (mineSpawnChance == 0){
            createMine(difficulty);
        }
        srand(seed + k);
        int fighterSpawnChance = rand() % level[difficulty].fighter;
        if (fighterSpawnChance == 1){
            createFighter(difficulty);
        }
        srand(seed + n);
        int cargoShipSpawnChance = rand() % level[difficulty].cargoShip;
        if (cargoShipSpawnChance == 0){
            createCargoShip(difficulty);
        }
        srand(seed + m);
        int bomberSpawnChance = rand() % level[difficulty].bomber;
        if (bomberSpawnChance == 0){
            createBomber(difficulty);
        }
        srand (seed + o);
        int droneSpawnChance = rand() % level[difficulty].drone;
        if (droneSpawnChance == 0){
            createDrone(difficulty);
        }
        i++; k++; m++, n++, o++;

        time_t current_time = time(NULL);
        score += (int) difftime(current_time, base_time);
        base_time = time(NULL);

        center = updateBoard(position, center, y_position, &EOG, &score);

        Sleep(65);
        system("@cls||clear");
    }
    // calculate the end time
    time_t end_time = time(NULL);

    int time_spent = (int) difftime(end_time, start_time);

    // refresh the screen to show game over and get name
    system("@cls||clear");

    printGameOver();

    printf("%s", TC_GRN);
    printf("%c[%d;%df", 0x1B, 20, 83);
    printf("Your score : %d", score);
    printf("%c[%d;%df", 0x1B, 23, 70);
    printf("What is your name ? (only in 3 characters)\n");
    printf("%c[%d;%df", 0x1B, 25, 89);
    printf("%s", TC_RED);
    char name[4] = {};
    scanf("%3s", name);
    printf("%s", TC_NRM);

    for (int j = 0; j < 3; ++j) {
        name[j] = toupper(name[j]);
    }

    // adding the score to the scores TXT file
    FILE * fp;
    fp = fopen("scores.txt", "a");
    if (fp == NULL){
        printf("Could not open a text file for score.\n");
        sleep(3);
        exit(-1);
    }

    //getting the time
    time_t current_time;
    struct tm *time_info;

    time(&current_time);
    time_info = localtime(&current_time);

    fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d %-4d %s %.4d\n", time_info->tm_year + 1900,
            time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour, time_info->tm_min,
            time_info->tm_sec, time_spent, name, score);

    fclose(fp);

    // free all the nodes for enemies to reset the game
    clearAllEnemies();
    clearAllBullets();


    system("cls");
    printf("%s", TC_GRN);

    char command;
    int option = 0;

    while (1){
        if (option == 0) {
            showRetry();
        } else{
            showGoToMenu();
        }

        command = _getch();
        system("@cls||clear");

        if (command == '\r'){
            printf("%s", TC_NRM);
            return option;
        }

        if (command == 80) {
            option++;
        } else if (command == 72) {
            option--;
        }

        // Ensure position stays within the valid range (0 to 1)
        if (option == -1) {
            option = 1;
        } else if (option == 2) {
            option = 0;
        }
    }
}


void printGameOver(){
    printf("%s", TC_B_CYN);
    char * gameOver = "   _____                           ____                      _ \n"
                      "  / ____|                         / __ \\                    | |\n"
                      " | |  __   __ _  _ __ ___    ___ | |  | |__   __ ___  _ __  | |\n"
                      " | | |_ | / _` || '_ ` _ \\  / _ \\| |  | |\\ \\ / // _ \\| '__| | |\n"
                      " | |__| || (_| || | | | | ||  __/| |__| | \\ V /|  __/| |    |_|\n"
                      "  \\_____| \\__,_||_| |_| |_| \\___| \\____/   \\_/  \\___||_|    (_)\n"
                      "                                                               \n"
                      "                                                               \n";
    int j = 0;
    for (int i = 0; i < 8; i++){
        printf("%c[%d;%df", 0x1B, i + 3, 60);
        while (gameOver[j++] != '\n'){
            printf("%c", gameOver[j]);
        }
    }
}

void showRetry(){
    printf("%c[%d;%df %s>>>%s\n", 0x1B, 26, 82, TC_RED, TC_GRN);
    printf("%c[%d;%df %sRetry%s\n", 0x1B, 26, 85, TC_RED, TC_GRN);
    printf("%c[%d;%df Menu\n", 0x1B, 27, 85);
}

void showGoToMenu(){
    printf("%c[%d;%df Retry\n", 0x1B, 26, 85);
    printf("%c[%d;%df %s>>>%s\n", 0x1B, 27, 82, TC_RED, TC_GRN);
    printf("%c[%d;%df %sMenu%s\n", 0x1B, 27, 85, TC_RED, TC_GRN);
}