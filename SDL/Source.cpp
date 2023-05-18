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
#include "game.h"
using namespace std;



int main(int argc, char** argv) {
	SetConsoleCP(1252);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Mix_Init(0);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_Music* fon = NULL;
	loadmusic(fon);
	int vol = 772;
	Mix_VolumeMusic(128);
	SDL_Window* window = SDL_CreateWindow(u8"¬≈“ ¿", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 700, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	TTF_Font* my_font = TTF_OpenFont("Text.ttf", 100);

	SDL_Texture* backgroundTexture = createIMG(renderer, "background\\background.jpg");
	SDL_Texture* button_playTexture = createIMG(renderer, "button\\button_play.png");
	SDL_Texture* record_button = createIMG(renderer, "button\\record_button.png");
	SDL_Texture* setting_button = createIMG(renderer, "button\\button_setting.png");
	SDL_Texture* exit_button = createIMG(renderer, "button\\button_exit.png");
	SDL_Texture* exit_confirmation = createIMG(renderer, "background\\confirmation_quit.png");

	SDL_Rect backgroung_rect = { 0, 0, 1200, 700 }, play_button_rect = { 457, 239, 286, 88 }, record_buuton_rect = { 457, 348, 286, 88 }, 
		setting_buuton_rect = { 457, 457, 286, 88 }, exit_buuton_rect = { 457, 566, 286, 88 }, confirmation_quit_rect = {362,174, 477, 352};
	int mx, my;
	SDL_Event event;
	bool quit = false, isQuit = false;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) quit = true;
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (!isQuit) {
					if (is_button_hit(play_button_rect, event.button.x, event.button.y)) choose_level(renderer, vol);
					if (is_button_hit(record_buuton_rect, event.button.x, event.button.y)) record(renderer);
					if (is_button_hit(setting_buuton_rect, event.button.x, event.button.y)) setting(renderer, vol);
					if (is_button_hit(exit_buuton_rect, event.button.x, event.button.y)) isQuit = true;
				}
				else {
					if (is_button_hit({ 454,364, 90, 69 }, event.button.x, event.button.y)) {
						SDL_DestroyTexture(backgroundTexture);
						SDL_DestroyTexture(button_playTexture);
						SDL_DestroyTexture(record_button);
						SDL_DestroyTexture(setting_button);
						SDL_DestroyTexture(exit_button);
						SDL_DestroyTexture(exit_confirmation);
						TTF_CloseFont(my_font);
						Mix_FreeMusic(fon);
						Mix_CloseAudio();
						TTF_Quit();
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						SDL_Quit();
						exit(1);
					}
					if (is_button_hit({ 651,364, 90, 69 }, event.button.x, event.button.y)) isQuit = false;
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_GetMouseState(&mx, &my);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroung_rect);
		if (!isQuit) {
			if (is_button_hit(play_button_rect, mx, my)) SDL_RenderCopy(renderer, button_playTexture, NULL, &play_button_rect);
			if (is_button_hit(record_buuton_rect, mx, my)) SDL_RenderCopy(renderer, record_button, NULL, &record_buuton_rect);
			if (is_button_hit(setting_buuton_rect, mx, my)) SDL_RenderCopy(renderer, setting_button, NULL, &setting_buuton_rect);
			if (is_button_hit(exit_buuton_rect, mx, my)) SDL_RenderCopy(renderer, exit_button, NULL, &exit_buuton_rect);
		}
		else {
			SDL_RenderCopy(renderer, exit_confirmation, NULL, &confirmation_quit_rect);
		}
		SDL_RenderPresent(renderer);
	}
	return 0;
}


