#include <iostream>
#include <unistd.h>

struct point {
    unsigned is_live:1;
};

// Ширина игрового поля
#define __WORLD_HEIGHT__ 40

// Высота игрового поля
#define __WORLD_WIDTH__ 40

// Игровое поле размером 40x40 клеток
point world[__WORLD_WIDTH__][__WORLD_HEIGHT__];

void initWorld(point world[][__WORLD_HEIGHT__])
{


    unsigned int i, j;
    srand(time(0));
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            unsigned int num = rand() % 1000 + 1;
            if (num % 2 == 0) {
                world[i][j].is_live = 1;
            } else {
                world[i][j].is_live = 0;
            }
        }
    }
}

unsigned int countLivePoints(point world[][__WORLD_HEIGHT__])
{
    unsigned int count = 0;
    unsigned i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (world[i][j].is_live == 1) {
                count++;
            }
        }
    }
    return count;
}

void readPointNeighbors(signed int nb[][2], unsigned int x, unsigned int y)
{
    unsigned int i, j;
    unsigned int k = 0;

    for (i = x - 1; i <= x + 1; i++) {
        for (j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y) {
                continue;
            }
            nb[k][0] = i;
            nb[k][1] = j;
            k++;
        }
    }
}

unsigned int countLiveNeighbors(point world[][__WORLD_HEIGHT__], unsigned int x, unsigned int y)
{
    unsigned int count = 0;
    unsigned int i;
    signed int nb[8][2];
    signed int _x, _y;

    readPointNeighbors(nb, x, y);

    for (i = 0; i < 8; i++) {
        _x = nb[i][0];
        _y = nb[i][1];

        if (_x < 0 || _y < 0) {
            continue;
        }
        if (_x >= __WORLD_WIDTH__ || _y >= __WORLD_HEIGHT__) {
            continue;
        }

        if (world[_x][_y].is_live == 1) {
            count++;
        }
    }

    return count;
}

void nextGeneration(point world[][__WORLD_HEIGHT__], point prev_world[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    unsigned int live_nb;
    point p;

    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            p = prev_world[i][j];
            live_nb = countLiveNeighbors(prev_world, i, j);

            if (p.is_live == 0) {
                if (live_nb == 3) {
                    world[i][j].is_live = 1;
                }
            } else {
                if (live_nb < 2 || live_nb > 3) {
                    world[i][j].is_live = 0;
                }
            }
        }
    }
}


void copyWorld(point src[][__WORLD_HEIGHT__], point dest[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void printWorld(point world[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (world[i][j].is_live == 1) {
                std::cout << '*';
            } else {
                std::cout << ' ';
            }
            std::cout << ' ';
        }
        std::cout << std::endl;
    }
}

int main()
{
    point world[__WORLD_WIDTH__][__WORLD_HEIGHT__];
    point prev_world[__WORLD_WIDTH__][__WORLD_HEIGHT__];

    initWorld(world);
    unsigned int live_points = -1;


    while (live_points != 0 )
    {
        printWorld(world);
        copyWorld(world, prev_world);
        nextGeneration(world, prev_world);


        live_points = countLivePoints(world);

        if (live_points == 0) {
            std::cout << "All points have died" << std::endl;
        }
        sleep(1);
    }

    return 0;
}