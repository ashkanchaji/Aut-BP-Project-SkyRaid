#include "board.h"
#include "color.h"


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>
#include <windows.h>

struct {
    int x;
    int y;
    int health;
    int bulletLevel; // 1 2 3
    int bulletDamage;// 2 4 6
}plane;

int rand_count = 0;

struct node * headOfEnemies = NULL;
struct bullet * headOfBullets = NULL;

void createBoard(){
    printCorners();

    int center = (WIDTH / 2);

    plane.x = center;
    plane.y = HEIGHT - 6;
    plane.health = 100;
    plane.bulletLevel = 1;
    plane.bulletDamage = 2;

    printSpaceShip(plane.y, plane.x);
}

//should be updated with enemies
int updateBoard(int position, int center, int y_position, bool * EOG, int * score){
    printCorners();
    // update the plane location

    center = (WIDTH / 2) + position;

    plane.x = center;
    plane.y = y_position;

    if (plane.health <= 0){
        plane.health = 0;
        *EOG = true;
        return center;
    }

    // update the game info
    if (*score < 0){
        *score = 0;
    }

    int level = 1;
    if (*score >= 100){
        if (*score >= 1000){
            level = 5;
        } else if (*score >= 500){
            level = 4;
        } else if (*score >= 250){
            level = 3;
        } else{
            level = 2;
        }
    }
    gotoxy(5, 154);
    printf("Score : %.4d", *score);
    gotoxy(7, 154);
    printf("Health : %3d", plane.health);
    gotoxy(9, 154);
    printf("Bullet Level : %d", plane.bulletLevel);
    gotoxy(11, 154);
    printf("Level %d", level);

    checkCollision();
    updateBullets();
    updateEnemies(score);

    for (struct node *current = headOfEnemies; current != NULL; current = current->next) {
        if (strcmp(current->type, "cargoShip") == 0){
            printCargoShip(current->y, current->x, current->health, current->maxHealth);
        }
        else if (strcmp(current->type, "package") == 0){
            printPackage(current->y, current->x, current->health, current->maxHealth);
        }
        else if (strcmp(current->type, "bomber") == 0){
            printBomber(current->y, current->x, current->health, current->maxHealth);
        }
        else if (strcmp(current->type, "bomb") == 0){
            printBomb(current->y, current->x, current->health, current->maxHealth);
        }
        else if (strcmp(current->type, "mine") == 0) {
            printMine(current->y, current->x, current->health, current->maxHealth);
        }
        else if (strcmp(current->type, "fighter") == 0){
            printFighter(current->y, current->x, current->health, current->maxHealth);
        }
        else if (strcmp(current->type, "bullet") == 0){
            gotoxy(current->y, current->x);
            printf("%s", TC_RED);
            putchar(':');
            printf("%s", TC_NRM);
        }
        else {
            printDrone(current->y, current->x, current->health, current->maxHealth);
        }
    }

    printSpaceShip(y_position, center);

    return center;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void printSpaceShip(int y_position ,int center){
    printf("%s", TC_B_CYN);
    char * spaceShip = " !/-\\! \n"
                       "/_\\0/_\\\n"
                       "\\'/:\\'/\n"
                       " ^^^^^ \n";
    int j = 0;
    for (int i = 0; i < 4; i++){
        gotoxy(y_position + i, center - 3);
        while (spaceShip[j] != '\n'){
            putchar(spaceShip[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printMine(int y, int x, int health, int maxHealth){
    if (((float) 2 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_GRN);
    } else if (((float) 1 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_YEL);
    } else {
        printf("%s", TC_RED);
    }

    char * mine = " _!_ \n"
                  "(_o_)\n"
                  " ''' \n";

    int j = 0;
    for (int i = 0; i < 3; ++i) {
        gotoxy(y + i, x - 2);
        while (mine[j] != '\n'){
            putchar(mine[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printCargoShip (int y, int x, int health, int maxHealth){
    if (((float) 2 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_GRN);
    } else if (((float) 1 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_YEL);
    } else {
        printf("%s", TC_RED);
    }
    char * cargoShip = "      ___      \n"
                       "      \\ /      \n"
                       "      | |      \n"
                       "_____/   \\_____\n"
                       "\\____     ____/\n"
                       "     |___|     \n"
                       "      \\ /      \n"
                       "     --*--     \n";

    int j = 0;
    for (int i = 0; i < 8; i++){
        gotoxy(y + i, x - 7);
        while (cargoShip[j] != '\n'){
            putchar(cargoShip[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printPackage (int y, int x, int health, int maxHealth){
    if (((float) 2 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_GRN);
    } else if (((float) 1 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_YEL);
    } else {
        printf("%s", TC_RED);
    }
    char * package = " _____ \n"
                     "|\\ | /|\n"
                     "|-(P)-|\n"
                     "|/_|_\\|\n";

    int j = 0;
    for (int i = 0; i < 4; i++){
        gotoxy(y + i, x - 3);
        while (package[j] != '\n'){
            putchar(package[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printBomber (int y, int x, int health, int maxHealth){
    if (((float) 2 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_GRN);
    } else if (((float) 1 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_YEL);
    } else {
        printf("%s", TC_RED);
    }
    char * bomber = "       /'7   \n"
                    "      /_<| ._\n"
                    " __,//   \\/ 7\n"
                    "<:_         =\n"
                    "   `\\\\ _ /\\ \\\n"
                    "      \\ <| '-\n"
                    "       \\.7   \n";

    int j = 0;
    for (int i = 0; i < 7; i++){
        gotoxy(y + i, x - 6);
        while (bomber[j] != '\n'){
            putchar(bomber[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printBomb (int y, int x, int health, int maxHealth){
    if (((float) 2 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_GRN);
    } else if (((float) 1 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_YEL);
    } else {
        printf("%s", TC_RED);
    }
    char * bomb = "|\\*/|\n"
                  "\\ = /\n"
                  " | | \n"
                  " | | \n"
                  " \\_/ \n";

    int j = 0;
    for (int i = 0; i < 5; i++){
        gotoxy(y + i, x - 2);
        while (bomb[j] != '\n'){
            putchar(bomb[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printFighter(int y, int x, int health, int maxHealth){
    if (((float) 2 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_GRN);
    } else if (((float) 1 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_YEL);
    } else {
        printf("%s", TC_RED);
    }
    char * fighter = " _   _ \n"
                     "/*\\^/*\\\n"
                     "\\ |=| /\n"
                     " \\\\0// \n"
                     "  \\./  \n";

    int j = 0;
    for (int i = 0; i < 5; i++){
        gotoxy(y + i, x - 3);
        while (fighter[j] != '\n'){
            putchar(fighter[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printDrone(int y, int x, int health, int maxHealth){
    if (((float) 2 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_GRN);
    } else if (((float) 1 / 3) * 100 <= ((float) health / maxHealth) * 100){
        printf("%s", TC_YEL);
    } else {
        printf("%s", TC_RED);
    }
    char * drone = "__ __\n"
                   " _!_ \n"
                   "|___|\n"
                   " /-\\ \n";

    int j = 0;
    for (int i = 0; i < 4; i++){
        gotoxy(y + i, x - 2);
        while (drone[j] != '\n'){
            putchar(drone[j++]);
        }
        j++;
    }
    printf("%s", TC_NRM);
}

void printCorners(){
    gotoxy(0, 0);
    printf("+ -");
    gotoxy(0, WIDTH - 3);
    printf("- +");
    gotoxy(HEIGHT - 1, 0);
    printf("+ -");
    gotoxy(HEIGHT - 1, WIDTH - 3);
    printf("- +");

    gotoxy(1, 0);
    putchar('|');
    gotoxy(1, WIDTH - 1);
    putchar('|');
    gotoxy(HEIGHT - 2, 0);
    putchar('|');
    gotoxy(HEIGHT - 2, WIDTH - 1);
    putchar('|');
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void createMine(int level){
    time_t t = time(NULL);
    srand(t);
    int position = (rand() % 11) * 7;
    int unary = rand() % 2;
    if (unary == 0){
        position *= -1;
    }

    int x = (WIDTH / 2) + position;

    for(struct node * currnt = headOfEnemies; currnt != NULL; currnt = currnt->next){
        if (currnt->x == x){
            return;
        }
        if (strcmp(currnt->type, "cargoShip") == 0 || strcmp(currnt->type, "bomber") == 0){
            if ( x == currnt->x + 7 || x == currnt->x - 7){
                return;
            }
        }
    }

    int health;
    if (level == 0){
        health = 2;
    } else if (level == 1){
        health = 4;
    } else {
        health = 6;
    }

    struct node * mine = createEnemy("mine", x, 1, health, health);
    addNewEnemy(&headOfEnemies, mine);
}

void createCargoShip(int level){
    time_t t = time(NULL);
    srand(t);
    int position = (rand() % 4) * 21;
    int unary = rand() % 2;
    if (unary == 0){
        position *= -1;
    }

    int x = (WIDTH / 2) + position;

    for(struct node * current = headOfEnemies; current != NULL; current = current->next){
        if ((current->x == x || current->x == x + 7 || current->x == x - 7) && current->y <= 9){
            return;
        }
    }

    int health;
    if (level == 0){
        health = 8;
    } else if (level == 1){
        health = 16;
    } else {
        health = 24;
    }

    struct node * cargoShip = createEnemy("cargoShip", x, 1, health, health);
    addNewEnemy(&headOfEnemies, cargoShip);
}

void createPackage (int y, int x){
    struct node * package = createEnemy("package", x, y, 6, 6);
    addNewEnemy(&headOfEnemies, package);
}

void createBomber(int level){
    int x = (WIDTH / 2) + (3 * 21);

    for(struct node * current = headOfEnemies; current != NULL; current = current->next){
        if (((current->x == x || current->x == x + 7 || current->x == x - 7) && current->y <= 9) ||
            strcmp(current->type, "bomber") == 0){
            return;
        }
    }

    int health;
    if (level == 0){
        health = 8;
    } else if (level == 1){
        health = 12;
    } else {
        health = 16;
    }

    struct node * bomber = createEnemy("bomber", x, 1, health, health);
    bomber->bomb = false;
    addNewEnemy(&headOfEnemies, bomber);
}

void createBomb(int y, int x){
    struct node * bomb = createEnemy("bomb", x, y, 4, 4);
    addNewEnemy(&headOfEnemies, bomb);
}

void createFighter(int level){
    time_t t = time(NULL);
    srand(t);
    int position = (rand() % 11) * 7;
    int unary = rand() % 2;
    if (unary == 0){
        position *= -1;
    }

    int x = (WIDTH / 2) + position;

    for(struct node * currnt = headOfEnemies; currnt != NULL; currnt = currnt->next){
        if (currnt->x == x){
            return;
        }
        if (strcmp(currnt->type, "cargoShip") == 0 || strcmp(currnt->type, "bomber") == 0){
            if ( x == currnt->x + 7 || x == currnt->x - 7){
                return;
            }
        }
    }

    int health;
    if (level == 0){
        health = 6;
    } else if (level == 1){
        health = 8;
    } else {
        health = 12;
    }

    struct node * fighter = createEnemy("fighter", x, 1, health, health);
    addNewEnemy(&headOfEnemies, fighter);
}

void createFighterBullet(int y, int x){
    struct node * bullet = createEnemy("bullet", x, y, 1, 1);
    addNewEnemy(&headOfEnemies, bullet);
}

void createDrone(int level){
    time_t t = time(NULL);
    srand(t);
    int position = (rand() % 11) * 7;
    int unary = rand() % 2;
    if (unary == 0){
        position *= -1;
    }

    int x = (WIDTH / 2) + position;

    for(struct node * currnt = headOfEnemies; currnt != NULL; currnt = currnt->next){
        if (currnt->x == x){
            return;
        }
        if (strcmp(currnt->type, "cargoShip") == 0 || strcmp(currnt->type, "bomber") == 0){
            if ( x == currnt->x + 7 || x == currnt->x - 7){
                return;
            }
        }
    }

    int health;
    if (level == 0){
        health = 2;
    } else if (level == 1){
        health = 4;
    } else {
        health = 6;
    }

    struct node * drone = createEnemy("drone", x, 1, health, health);
    addNewEnemy(&headOfEnemies, drone);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void fireBullets (int center){
    struct bullet * bullet1 = createBullet(center - 2, plane.y - 1);
    struct bullet * bullet2 = createBullet(center + 2, plane.y - 1);

    addBullet(&headOfBullets, bullet1);
    addBullet(&headOfBullets, bullet2);
}

struct bullet * createBullet (int x, int y){
    struct bullet * bullet;
    bullet = (struct bullet *) malloc(sizeof (struct bullet));
    if (bullet == NULL){
        system("@cls||clear");
        printf("Memory allocation error!\n");
        exit(-1);
    }

    bullet->x = x;
    bullet->y = y;
    bullet->next = NULL;

    return bullet;
}

void addBullet(struct bullet ** head, struct bullet * newBullet){
    if (*head == NULL){
        *head = newBullet;
        return;
    }
    struct bullet * current;
    for(current = *head; current->next != NULL; current = current->next);

    current->next = newBullet;
}

void destroyBullet(struct bullet ** head, struct bullet * bullet){
    if (*head == NULL || bullet == NULL) {
        // Handle invalid arguments
        return;
    }

    if (*head == bullet) {
        // If the bullet is the head, update the head
        *head = bullet->next;
        free(bullet);
        return;
    }

    struct bullet * current;
    for (current = *head; current->next != NULL && current->next != bullet; current = current->next);

    if (current->next == NULL) {
        // bullet not found in the list
        return;
    }

    struct bullet * tmp = current->next;
    current->next = current->next->next;

    // Free memory for the node
    free(tmp);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct node * createEnemy(char * type, int x, int y, int health, int maxHealth){
    struct node * newEnemy;
    newEnemy = (struct node *) malloc(sizeof (struct node));
    if (newEnemy == NULL){
        system("@cls||clear");
        printf("Memory allocation error!\n");
        exit(-1);
    }

    newEnemy->type = strdup(type);
    newEnemy->x = x;
    newEnemy->y = y;
    newEnemy->health = health;
    newEnemy->maxHealth = maxHealth;
    newEnemy->bulletHit = false;
    newEnemy->next = NULL;

    return newEnemy;
}

void addNewEnemy(struct node ** head, struct node * newEnemy){
    if (*head == NULL){
        *head = newEnemy;
        return;
    }
    struct node * current;
    for(current = *head; current->next != NULL; current = current->next);

    current->next = newEnemy;
}

void destroyEnemy(struct node ** head, struct node * enemy){
    if (*head == NULL || enemy == NULL) {
        // Handle invalid arguments
        return;
    }

    if (*head == enemy) {
        // If the enemy is the head, update the head
        *head = enemy->next;
        free(enemy->type);
        free(enemy);
        return;
    }

    struct node* current;
    for (current = *head; current->next != NULL && current->next != enemy; current = current->next);

    if (current->next == NULL) {
        // Enemy not found in the list
        return;
    }

    struct node * tmp = current->next;
    current->next = current->next->next;

    // Free memory for type and the node
    free(tmp->type);
    free(tmp);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void clearAllEnemies(){
    struct node * current = headOfEnemies;
    struct node * nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    headOfEnemies = NULL; // Set the head back to NULL after freeing all nodes
}

void clearAllBullets(){
    struct bullet * current = headOfBullets;
    struct bullet * nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    headOfBullets = NULL; // Set the head back to NULL after freeing all nodes
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void gotoxy(int y, int x) {
    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void checkCollision(){
    for(struct node * current = headOfEnemies; current != NULL; current = current->next){
        if (strcmp(current->type, "drone") == 0){
            for (struct node * others = headOfEnemies; others != NULL; others = others->next){
                if (strcmp(others->type, "mine") == 0){
                    if ((others->y + 3 >= current->y && others->y <= current->y + 3) &&
                        abs(others->x - current->x) <= 3){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "fighter") == 0){
                    if ((others->y + 5 >= current->y && others->y <= current->y + 3) &&
                        abs(others->x - current->x) <= 4) {
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "bomber") == 0){
                    if ((others->y + 7 >= current->y && others->y <= current->y + 3) &&
                        abs(others->x - current->x) <= 7) {
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "package") == 0){
                    if ((others->y + 4 >= current->y && others->y <= current->y + 3) &&
                        abs(others->x - current->x) <= 4) {
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "drone") == 0 && (others->x != current->x)){
                    if ((others->y + 3 >= current->y && others->y <= current->y + 3) &&
                        abs(others->x - current->x) <= 4) {
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
            }
        }
        else if (strcmp(current->type, "bomber") == 0){
            for (struct node * others = headOfEnemies; others != NULL; others = others->next){
                if (strcmp(others->type, "mine") == 0){
                    if ((others->y + 3 >= current->y && others->y <= current->y + 6) &&
                        abs(others->x - current->x) <= 7){
                        others->health = 0;
                        current->health -= 5;
                        break;
                    }
                }
                else if (strcmp(others->type, "fighter") == 0){
                    if ((others->y + 5 >= current->y && others->y <= current->y + 6) &&
                        abs(others->x - current->x) <= 7){
                        others->health = 0;
                        current->health -= 8;
                        break;
                    }
                }
                else if (strcmp(others->type, "cargoShip") == 0){
                    if ((current->y + 6 >= others->y && current->y <= others->y + 7) &&
                        abs(others->x - current->x) <= 7){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "package") == 0){
                    if ((others->y + 4 >= current->y && others->y <= current->y + 6) &&
                        abs(others->x - current->x) <= 7){
                        others->health = 0;
                        current->health -= 2;
                        break;
                    }
                }
            }
        }
        else if (strcmp(current->type, "bomb") == 0){
            for (struct node * others = headOfEnemies; others != NULL; others = others->next){
                if (strcmp(others->type, "mine") == 0){
                    if ((current->y + 4 >= others->y && current->y + 4 <= others->y + 3) &&
                        others->x == current->x){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "fighter") == 0){
                    if ((current->y + 4 >= others->y && current->y + 4 <= others->y + 5) &&
                        others->x == current->x){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "cargoShip") == 0){
                    if ((current->y + 4 >= others->y && current->y + 4 <= others->y + 7) &&
                        abs(others->x - current->x) <= 7){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "package") == 0){
                    if ((current->y + 4 >= others->y && current->y + 4 <= others->y + 4) &&
                        others->x == current->x){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
            }
        }
        else if (strcmp(current->type, "cargoShip") == 0){
            for (struct node * others = headOfEnemies; others != NULL; others = others->next){
                if (strcmp(others->type, "mine") == 0){
                    if (((current->y + 7 >= others->y && current->y + 7 <= others->y + 3)
                         && current->x == others->x) ||
                        ((current->y + 5 >= others->y && current->y + 5 <= others->y + 3)
                         && abs(current->x - others->x) == 7 )){
                        others->health = 0;
                        current->health -= 8;
                        break;
                    }
                }
                else if (strcmp(others->type, "fighter") == 0){
                    if (((current->y + 7 >= others->y && current->y + 7 <= others->y + 5)
                         && current->x == others->x) ||
                        ((current->y + 5 >= others->y && current->y + 5 <= others->y + 5)
                         && abs(current->x - others->x) == 7 )){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "package") == 0){
                    if (((current->y + 7 >= others->y && current->y + 7 <= others->y + 4)
                         && current->x == others->x) ||
                        ((current->y + 5 >= others->y && current->y + 5 <= others->y + 4)
                         && abs(current->x - others->x) == 7 )){
                        others->health = 0;
                        current->health -= 4;
                        break;
                    }
                }
                else if (strcmp(others->type, "drone") == 0){
                    if (((current->y + 7 >= others->y && current->y + 7 <= others->y + 4)
                         && abs(current->x - others->x) <= 5) ||
                        ((current->y + 5 >= others->y && current->y + 5 <= others->y + 4)
                         && abs(current->x - others->x) <= 8 )){
                        others->health = 0;
                        current->health -= 4;
                        break;
                    }
                }
            }
        }
        else if (strcmp(current->type, "fighter") == 0){
            for (struct node * others = headOfEnemies; others != NULL; others = others->next){
                if (strcmp(others->type, "mine") == 0){
                    if ((current->y + 4 >= others->y && current->y + 4 <= others->y + 4) &&
                        others->x == current->x){
                        others->health = 0;
                        current->health -= 6;
                        break;
                    }
                }
                if (strcmp(others->type, "package") == 0){
                    if ((current->y + 4 >= others->y && current->y + 4 <= others->y + 4) &&
                        others->x == current->x){
                        others->health = 0;
                        current->health -= 4;
                        break;
                    }
                }
            }
        }
        else if (strcmp(current->type, "bullet") == 0){
            for (struct node * others = headOfEnemies; others != NULL; others = others->next){
                if (strcmp(others->type, "mine") == 0){
                    if ((current->y >= others->y && current->y <= others->y + 4) &&
                        others->x == current->x){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "fighter") == 0 || strcmp(others->type, "bomb") == 0){
                    if ((current->y >= others->y && current->y <= others->y + 5) &&
                        others->x == current->x){
                        if (strcmp(others->type, "bomb") == 0){
                            others->health = 0;
                            current->health = 0;
                        }else {
                            others->health -= 3;
                            current->health = 0;
                        }
                        break;
                    }
                }
                else if (strcmp(others->type, "package") == 0){
                    if ((current->y >= others->y && current->y <= others->y + 5) &&
                        others->x == current->x){
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "drone") == 0){
                    if ((current->y >= others->y && current->y <= others->y + 5) &&
                        abs(current->x - others->x) <= 5){
                        others->health -= 3;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "cargoShip") == 0){
                    if ((current->y >= others->y && current->y <= others->y + 8) &&
                        others->x == current->x){
                        others->health -= 3;
                        current->health = 0;
                        break;
                    }
                }
                else if (strcmp(others->type, "bomber") == 0){
                    if ((current->y >= others->y && current->y <= others->y + 7) &&
                        others->x == current->x){
                        others->health -= 3;
                        current->health = 0;
                        break;
                    }
                }
            }
        }
        else if (strcmp(current->type, "mine") == 0) {
            for (struct node *others = headOfEnemies; others != NULL; others = others->next) {
                if (strcmp(others->type, "package") == 0) {
                    if ((current->y + 2 >= others->y && current->y + 2 <= others->y + 4) &&
                        others->x == current->x) {
                        others->health = 0;
                        current->health = 0;
                        break;
                    }
                }
            }
        }
    }
}

// Modify the updateEnemies function
void updateEnemies(int * score) {
    struct node *current = headOfEnemies;
    struct node *nextEnemy;

    while (current != NULL) {
        // Save the next enemy before potentially destroying the current one
        nextEnemy = current->next;

        if (strcmp(current->type, "mine") == 0) {
            if (current->x == plane.x && (current->y >= plane.y && current->y <= plane.y + 4)) {
                plane.health -= 25;
                if (plane.bulletLevel != 1){
                    plane.bulletLevel -= 1;
                    plane.bulletDamage -= 2;
                }
                destroyEnemy(&headOfEnemies, current);
            } else if (current->y >= 46 || current->health <= 0) {
                if (current->health <= 0 && current->bulletHit == true){
                    *score += 10;
                }
                destroyEnemy(&headOfEnemies, current);
            } else {
                current->y += 1;
            }
        }
        else if (strcmp(current->type, "package") == 0){
            if (current->x == plane.x && current->y + 3 >= plane.y && current->y + 3 <= plane.y + 4){
                srand(time(NULL));
                int prize = rand() % 2;

                if (prize == 0){
                    plane.health += 15;
                    if (plane.health > 100){
                        plane.health = 100;
                    }
                } else {
                    if (plane.bulletLevel != 3){
                        plane.bulletLevel += 1;
                        plane.bulletDamage += 2;
                    }
                }
                destroyEnemy(&headOfEnemies, current);
            }
            else if (current->y + 3 >= 47 || current->health <= 0){
                destroyEnemy(&headOfEnemies, current);
            }
            else {
                current->y += 1;
            }
        }
        else if (strcmp(current->type, "cargoShip") == 0) {
            if ((current->x == plane.x && (current->y + 7 >= plane.y && current->y + 7 <= plane.y + 4)) ||
                (current->x + 7 == plane.x && (current->y + 4 >= plane.y && current->y + 4 <= plane.y + 4)) ||
                (current->x - 7 == plane.x && (current->y + 4 >= plane.y && current->y + 4 <= plane.y + 4))) {
                plane.health -= 10;
                if (plane.bulletLevel != 1){
                    plane.bulletLevel -= 1;
                    plane.bulletDamage -= 2;
                }
                destroyEnemy(&headOfEnemies, current);
            }
            else if (current->y + 7 >= 47 || current->health <= 0) {
                if (current->health <= 0 && current->bulletHit == true){
                    *score += 25;

                    createPackage(current->y + 3, current->x);
                }
                destroyEnemy(&headOfEnemies, current);
            } else {
                current->y += 2;
            }
        }
        else if (strcmp(current->type, "bullet") == 0) {
            if (current->x == plane.x && (current->y >= plane.y && current->y <= plane.y + 4)){
                plane.health -= 3;
                if (plane.bulletLevel != 1){
                    plane.bulletLevel -= 1;
                    plane.bulletDamage -= 2;
                }
                destroyEnemy(&headOfEnemies, current);
            } else if (current->y >= 47 || current->health <= 0){
                destroyEnemy(&headOfEnemies, current);
            } else {
                current->y += 3;
            }
        }
        else if (strcmp(current->type, "fighter") == 0){
            if (current->x == plane.x && (current->y + 4 >= plane.y && current->y + 4 <= plane.y + 4)) {
                plane.health -= 10;
                if (plane.bulletLevel != 1){
                    plane.bulletLevel -= 1;
                    plane.bulletDamage -= 2;
                }
                destroyEnemy(&headOfEnemies, current);
            } else if (current->y + 4 >= 47 || current->health <= 0) {
                if (current->health <= 0 && current->bulletHit == true){
                    *score += 20;
                }
                destroyEnemy(&headOfEnemies, current);
            } else {
                srand(time(NULL));
                int bulletSpawnChance = rand() % 3;
                if (bulletSpawnChance == 0){
                    createFighterBullet(current->y + 5, current->x);
                }

                current->y += 2;
            }
        }
        else if (strcmp(current->type, "bomb") == 0){
            if (current->x == plane.x && (current->y + 4 >= plane.y && current->y + 4 <= plane.y + 4)) {
                plane.health -= 50;
                if (plane.bulletLevel != 1){
                    plane.bulletLevel = 1;
                    plane.bulletDamage = 2;
                }
                destroyEnemy(&headOfEnemies, current);
            } else if (current->y + 4 >= 47 || current->health <= 0) {
                if (current->health <= 0 && current->bulletHit == true){
                    *score += 10;
                }
                destroyEnemy(&headOfEnemies, current);
            } else {
                current->y += 2;
            }
        }
        else if (strcmp(current->type, "bomber") == 0){
            if ((current->x == plane.x && (current->y + 6 >= plane.y && current->y + 6 <= plane.y + 4)) ||
                (current->x + 7 == plane.x && (current->y + 5 >= plane.y && current->y + 5 <= plane.y + 4)) ||
                (current->x - 7 == plane.x && (current->y + 3 >= plane.y && current->y + 3 <= plane.y + 4))) {
                plane.health -= 10;
                if (plane.bulletLevel != 1){
                    plane.bulletLevel -= 1;
                    plane.bulletDamage -= 2;
                }
                destroyEnemy(&headOfEnemies, current);
            }
            else if (current->y + 6 >= 47 || current->x < 20 || current->health <= 0) {
                if (current->health <= 0 && current->bulletHit == true){
                    *score += 20;
                }
                destroyEnemy(&headOfEnemies, current);
            } else {
                if (current->bomb == false){
                    if (rand_count > 7){
                        rand_count = 0;
                    }
                    srand(time(NULL) + rand_count);
                    int bombSpawnChance = rand() % 7;
                    if (bombSpawnChance == 0){
                        createBomb(current->y + 9, current->x);
                        current->bomb = true;
                    }
                    rand_count++;
                }

                current->y += 1;
                current->x -= 7;
            }
        }
        else if (strcmp(current->type, "drone") == 0){
            if (abs(current->x - plane.x) <= 6 && current->y + 3 >= plane.y && current->y + 3 <= plane.y + 4){
                plane.health -= 50;
                destroyEnemy(&headOfEnemies, current);
            }
            else if (current->health <= 0){
                if (current->health <= 0 && current->bulletHit == true){
                    *score += 25;
                }
                destroyEnemy(&headOfEnemies, current);
            }
            else {
                if (current->y < plane.y){
                    current->y += 1;
                }
                else if (current->y > plane.y){
                    current->y -= 1;
                }
                if (current->x < plane.x){
                    current->x += 1;
                }
                else if (current->x > plane.x){
                    current->x -= 1;
                }
            }
        }

        // Move to the next enemy
        current = nextEnemy;
    }
}

// Modify the updateBullets function
void updateBullets() {
    struct bullet *currentBullet = headOfBullets;
    struct bullet *nextBullet;

    while (currentBullet != NULL) {
        // Save the next bullet before potentially destroying the current one
        nextBullet = currentBullet->next;

        // Check if the bullet is out of bounds
        if (currentBullet->y <= 0) {
            destroyBullet(&headOfBullets, currentBullet);
        } else {
            bool bulletDestroyed = false;

            // Check for collisions with enemies
            for (struct node *enemy = headOfEnemies; enemy != NULL; enemy = enemy->next) {
                if (strcmp(enemy->type, "cargoShip") == 0) {
                    if (((abs(currentBullet->x - enemy->x) <= 5) && (currentBullet->y <= enemy->y + 4)) ||
                        ((abs(currentBullet->x - enemy->x) <= 2) && (currentBullet->y <= enemy->y + 7))) {
                        enemy->health -= plane.bulletDamage;
                        enemy->bulletHit = true;
                        destroyBullet(&headOfBullets, currentBullet);
                        bulletDestroyed = true;
                        break;
                    }
                }
                else if (strcmp(enemy->type, "fighter") == 0){
                    if ((abs(currentBullet->x - enemy->x) <= 2) && (currentBullet->y <= enemy->y + 3)) {
                        enemy->health -= plane.bulletDamage;
                        enemy->bulletHit = true;
                        destroyBullet(&headOfBullets, currentBullet);
                        bulletDestroyed = true;
                        break;
                    }
                }
                else if (strcmp(enemy->type, "bomb") == 0){
                    if ((abs(currentBullet->x - enemy->x) <= 2) && (currentBullet->y <= enemy->y + 1)) {
                        enemy->health -= plane.bulletDamage;
                        enemy->bulletHit = true;
                        destroyBullet(&headOfBullets, currentBullet);
                        bulletDestroyed = true;
                        break;
                    }
                }
                else if (strcmp(enemy->type, "bomber") == 0){
                    if ((currentBullet->x - enemy->x == 5 && currentBullet->y <= enemy->y + 5) ||
                        (currentBullet->x - enemy->x == -5 && currentBullet->y <= enemy->y + 3) ||
                        (currentBullet->x - enemy->x == -2 && currentBullet->y <= enemy->y + 4) ||
                        (currentBullet->x - enemy->x == 2 && currentBullet->y <= enemy->y + 6)) {
                        enemy->health -= plane.bulletDamage;
                        enemy->bulletHit = true;
                        destroyBullet(&headOfBullets, currentBullet);
                        bulletDestroyed = true;
                        break;
                    }
                }
                else if (strcmp(enemy->type, "package") == 0){
                    if ((abs(currentBullet->x - enemy->x) <= 2) && (currentBullet->y <= enemy->y + 3)) {
                        enemy->health -= 1;
                        enemy->bulletHit = true;
                        destroyBullet(&headOfBullets, currentBullet);
                        bulletDestroyed = true;
                        break;
                    }
                }
                else if (strcmp(enemy->type, "drone") == 0){
                    if ((abs(currentBullet->x - enemy->x) <= 2) && (currentBullet->y <= enemy->y + 3)){
                        enemy->health -= plane.bulletDamage;
                        enemy->bulletHit = true;
                        destroyBullet(&headOfBullets, currentBullet);
                        bulletDestroyed = true;
                        break;
                    }
                }
                else if (strcmp(enemy->type, "mine") == 0){
                    if ((abs(currentBullet->x - enemy->x) <= 2) && (currentBullet->y <= enemy->y + 2)) {
                        enemy->health -= plane.bulletDamage;
                        enemy->bulletHit = true;
                        destroyBullet(&headOfBullets, currentBullet);
                        bulletDestroyed = true;
                        break;
                    }
                }
            }

            if (!bulletDestroyed) {
                // Update the game board to clear the previous position of the bullet
                gotoxy(currentBullet->y, currentBullet->x);
                printf("%s", TC_B_WHT);
                putchar('.');
                printf("%s", TC_NRM);
                // Move the bullet
                currentBullet->y -= 3;
            }
        }

        // Move to the next bullet
        currentBullet = nextBullet;
    }
}

