#pragma once
#pragma comment(lib, "SDL2_mixer.lib")
#include "SDL.h"
#include "stdlib.h"
#include <iostream>
#include <SDL_TTF.h>
#include "SDL_image.h"
#include <ctime>
#include <Windows.h>
#include "SDL_mixer.h"
#include <fstream>
#include <math.h>
#include <string>
using namespace std;

void drawText(SDL_Renderer*& renderer, SDL_Texture*& texture, char* text, TTF_Font* font, SDL_Color back_color, SDL_Color fore_color, SDL_Rect rect);
bool is_button_hit(SDL_Rect button, int x, int y);
void sound(Mix_Chunk* Sound);
void loadmusic(Mix_Music* fon);
void ÑhangeLineFile(string line_add, string name, int number_line);
bool is_ball_hit(SDL_Rect ball, int x, int y);
SDL_Texture* createIMG(SDL_Renderer*& renderer, string name);
void clean_record();
double pw(int x1, int y1, int x2, int y2, int x3, int y3);
bool is_triangle_hit(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);
void setting(SDL_Renderer*& renderer, int& vol);
void record(SDL_Renderer*& renderer);
void level(SDL_Renderer*& renderer, bool& check_quit, int vol, int number_level, const char* branch1, const char* branch2, const char* branch3, const char* branch4, const char* branch5,
	const char* background_name, const char* name_level);
void choose_level(SDL_Renderer*& renderer, int vol);