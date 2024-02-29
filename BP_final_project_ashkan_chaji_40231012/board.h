#ifndef PROJECTFILE_LIBRARY_H
#define PROJECTFILE_LIBRARY_H

#include <stdbool.h>

#define HEIGHT 51
#define WIDTH 152

struct node{
    char * type;
    int x;
    int y;
    int health;
    int maxHealth;
    bool bomb;
    bool bulletHit;
    struct node * next;
};

struct bullet{
    int x;
    int y;
    struct bullet * next;
};


void gotoxy(int y, int x);

void printSpaceShip(int Y_position, int center);
void printMine(int y, int x, int health, int maxHealth);
void printCargoShip (int y, int x, int health, int maxHealth);
void printPackage (int y, int x, int health, int maxHealth);
void printBomber (int y, int x, int health, int maxHealth);
void printBomb (int y, int x, int health, int maxHealth);
void printFighter(int y, int x, int health, int maxHealth);
void printDrone(int y, int x, int health, int maxHealth);

struct node * createEnemy(char * type, int x, int y, int health, int maxHealth);
void addNewEnemy(struct node ** head, struct node * newEnemy);
void destroyEnemy(struct node ** head, struct node * enemy);

void clearAllEnemies();
void clearAllBullets();

void createMine(int level);
void createCargoShip(int level);
void createPackage (int y, int x);
void createBomber(int level);
void createBomb(int y, int x);
void createFighter(int level);
void createFighterBullet(int y, int x);
void createDrone(int level);

struct bullet * createBullet (int x, int y);
void addBullet(struct bullet ** head, struct bullet * newBullet);
void destroyBullet(struct bullet ** head, struct bullet * bullet);
void fireBullets (int center);

void checkCollision();
void updateEnemies(int * score);
void updateBullets();

void printCorners();
void createBoard();
int updateBoard(int position, int center, int y_position, bool * EOG, int * score);

#endif //PROJECTFILE_LIBRARY_H
