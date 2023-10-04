#include "life.h"
#include <stdlib.h>
#include <SFML/Graphics.hpp>


void DrawGen(unsigned* gen)
{

}

void CalcNextGen(unsigned* current_gen, unsigned* next_gen)
{

}


int main()
{
    unsigned current_gen[SIZE_X * SIZE_Y];
    unsigned next_gen[SIZE_X * SIZE_Y];


    for (unsigned y = 0; y < SIZE_Y; y++)
        for (unsigned x = 0; x < SIZE_X; x++)
        {
            current_gen[y * SIZE_X + x] = rand() % 2;
        }

    DrawGen(current_gen);

    for (unsigned i = 0; i < TOTAL_TICS; i++)
    {
        ClacNextGen(current_gen, next_gen);
        DrawGen(next_gen);
        unsigned *tmp = current_gen;
        current_gen = next_gen;
        next_gen = tmp;
    }

    return 0;
}