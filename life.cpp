#include <cstdlib>
#include <cstring> 
#include "sim.h"

void DrawGen();
void CalcNextGen();

int current_gen[SIZE_X * SIZE_Y];
int next_gen[SIZE_X * SIZE_Y];

void DrawGen() 
{
    for (int y = 0; y < SIZE_Y; y++) 
    {
        for (int x = 0; x < SIZE_X; x++) 
        {
            if (next_gen[y * SIZE_X + x] == 1) 
            {
                SimPutPixel(x, y, 0xFFFF0000);
            }
        }
    }
    SimFlush();
}


void CalcNextGen()
{
    for (int y = 0; y < SIZE_Y; y++) 
    {
        for (int x = 0; x < SIZE_X; x++) 
        {
            int neighbors = 0;
            for (int i = -1; i <= 1; i++) 
            {
                for (int j = -1; j <= 1; j++) 
                {
                    if (i == 0 && j == 0) continue;
                    int nx = x + i;
                    int ny = y + j;

                    if (nx >= 0 && nx < SIZE_X && ny >= 0 && ny < SIZE_Y) 
                    {
                        neighbors += current_gen[ny * SIZE_X + nx];
                    }
                }
            }


            if (current_gen[y * SIZE_X + x] == 1) 
            {
                if (neighbors < 2 || neighbors > 3) 
                {
                    next_gen[y * SIZE_X + x] = 0;
                } else 
                {
                    next_gen[y * SIZE_X + x] = 1;
                }
            } else 
            {
                if (neighbors == 3) 
                {
                    next_gen[y * SIZE_X + x] = 1;
                } else 
                {
                    next_gen[y * SIZE_X + x] = 0;
                }
            }
        }
    }
}


void app()
{
    for (int y = 0; y < SIZE_Y; y++) 
    {
        for (int x = 0; x < SIZE_X; x++) {
            current_gen[y * SIZE_X + x] = SimRand() % 2;
        }
    }

    for (int steps = 0; steps < MAX_STEPS; steps++)
    {
        CalcNextGen();
        DrawGen();

        
        for (int x = 0; x < SIZE_X; x++) 
        {
            for (int y = 0; y < SIZE_Y; y++) 
            {
                current_gen[y * SIZE_X + x] = next_gen[y * SIZE_X + x];
            }
        }
    }

    return;
}