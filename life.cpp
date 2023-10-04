#include "life.h"
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <cstring> 


void DrawGen(uint8_t* gen, sf::RenderWindow& window) {
    window.clear();
    sf::RectangleShape cell(sf::Vector2f(1, 1));

    for (int y = 0; y < SIZE_Y; y++) 
    {
        for (int x = 0; x < SIZE_X; x++) 
        {
            if (gen[y * SIZE_X + x] == 1) 
            {
                cell.setPosition(x, y);
                window.draw(cell);
            }
        }
    }
    window.display();
}

void CalcNextGen(uint8_t* current_gen, uint8_t* next_gen)
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


int main()
{
    sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "Game of Life");

    uint8_t current_gen[SIZE_X * SIZE_Y];
    uint8_t next_gen[SIZE_X * SIZE_Y];

    for (int y = 0; y < SIZE_Y; y++) 
    {
        for (int x = 0; x < SIZE_X; x++) {
            current_gen[y * SIZE_X + x] = std::rand() % 2;
        }
    }

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
        }

        CalcNextGen(current_gen, next_gen);
        DrawGen(next_gen, window);

        std::swap(current_gen, next_gen);
    }

    return 0;
}