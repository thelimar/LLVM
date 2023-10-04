#pragma once

#include <SFML/Graphics.hpp>

#define SIZE_X 600
#define SIZE_Y 400

void CalcNextGen(uint8_t* current_gen, uint8_t* next_gen);
void DrawGen(uint8_t* gen, sf::RenderWindow& window);
