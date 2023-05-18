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

void drawText(SDL_Renderer*& renderer, SDL_Texture*& texture, char* text, TTF_Font* font, SDL_Color back_color, SDL_Color fore_color, SDL_Rect rect) {
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Shaded(font, text, fore_color, back_color);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}

bool is_button_hit(SDL_Rect button, int x, int y) {
	if ((x > button.x) && (x < button.x + button.w) && (y > button.y) && (y < button.y + button.h)) return TRUE;
	return FALSE;
}

void sound(Mix_Chunk* Sound)
{
	Mix_PlayChannel(-1, Sound, 0);
}

void loadmusic(Mix_Music* fon)
{
	fon = Mix_LoadMUS("music\\background.mp3");
	Mix_PlayMusic(fon, -1);
}

void ÑhangeLineFile(string line_add, string name, int number_line) {
	ifstream file;
	file.open(name);
	int i = 0;
	string line;
	string new_file;
	while (getline(file, line)) {
		i++;
		if (!(i == number_line)) {
			new_file.insert(new_file.size(), line);
			new_file.insert(new_file.size(), "\r\n");
		}
		else {
			new_file.insert(new_file.size(), line_add);
			new_file.insert(new_file.size(), "\r\n");
		}
	}
	file.close();
	ofstream file_out;
	file_out.open(name, std::ios::trunc | std::ios::binary);
	file_out.write(new_file.c_str(), new_file.size());
	file_out.clear();
}

bool is_ball_hit(SDL_Rect ball, int x, int y) {
	if (ball.w == 0) return false;
	int centerX = ball.x + ball.w / 2;
	int centerY = ball.y + ball.h / 2;
	int radius = ball.w / 2;
	return sqrt((centerX - x) * (centerX - x) + (centerY - y) * (centerY - y)) < radius;
}

SDL_Texture* createIMG(SDL_Renderer*& renderer, string name) {
	SDL_Surface* Surface = IMG_Load(name.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, Surface);
	SDL_FreeSurface(Surface);
	return texture;
}

void clean_record() {
	ofstream file;
	file.open("record.txt", ofstream::out | ofstream::trunc);
	for (int i = 0; i < 12; i++) file << "0" << "\n";
	file.clear();
}

double pw(int x1, int y1, int x2, int y2, int x3, int y3) {
	return abs(x2 * y3 - x3 * y2 - x1 * y3 + x3 * y1 + x1 * y2 - x2 * y1);
}

bool is_triangle_hit(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
	double sum = pw(x, y, x1, y1, x2, y2) + pw(x, y, x2, y2, x3, y3) + pw(x, y, x3, y3, x1, y1);
	return abs(pw(x1, y1, x2, y2, x3, y3) - sum) <= 0.01;
}

void setting(SDL_Renderer*& renderer, int& vol) {
	SDL_Texture* settingTexture = createIMG(renderer, "background\\setting.jpg");
	SDL_Texture* ballTexture = createIMG(renderer, "texture\\ball.png");
	SDL_Texture* exit_buttonTexture = createIMG(renderer, "button\\button_back.png");
	SDL_Texture* button_recordTexture = createIMG(renderer, "button\\button_record.png");
	SDL_Texture* confirmation_quit = createIMG(renderer, "background\\confirmation_record.png");
	bool is_ball = false;
	SDL_Rect backgroung_rect = { 0, 0,1200, 700 }, ball_rect = { vol,237,17,17 }, button_exit_rect = { 436, 419, 337, 77 }, button_record_rect = { 435, 305, 337, 77 },
		confirmation_quit_rect = { 362,174, 477, 352 };
	SDL_Event event;
	int mx, my;
	bool quit = false, isQuit = false;
	while (1) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) exit(1);
			if (!isQuit) {
				if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_LMASK) {
					if ((mx > 436) && (mx < 772) && (my > 232) && (my < 258)) {
						ball_rect.x = mx - ball_rect.h / 2;
						is_ball = true;
						vol = mx;
						Mix_VolumeMusic(int((1 - (double(772 - mx) / (772 - 436))) * 128));
					}
					else if ((mx > 436) && (mx < 772) && is_ball) {
						ball_rect.x = mx - ball_rect.h / 2;
						vol = mx;
						Mix_VolumeMusic(int((1 - (double(772 - mx) / (772 - 436))) * 128));
					}
				}
				else is_ball = false;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (!isQuit) {
					if (is_button_hit(button_exit_rect, event.button.x, event.button.y)) {
						SDL_DestroyTexture(settingTexture);
						SDL_DestroyTexture(ballTexture);
						SDL_DestroyTexture(exit_buttonTexture);
						SDL_DestroyTexture(button_recordTexture);
						SDL_DestroyTexture(confirmation_quit);
						return;
					}
					if (is_button_hit(button_record_rect, event.button.x, event.button.y)) isQuit = true;
				}
				else {
					if (is_button_hit({ 454,364, 90, 69 }, event.button.x, event.button.y)) {
						clean_record();
						isQuit = false;
					}
					if (is_button_hit({ 651,364, 90, 69 }, event.button.x, event.button.y)) isQuit = false;
				}
			}
		}
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, settingTexture, NULL, &backgroung_rect);
		if (!isQuit) {
			SDL_RenderCopy(renderer, ballTexture, NULL, &ball_rect);
			SDL_GetMouseState(&mx, &my);
			if (is_button_hit(button_exit_rect, mx, my)) SDL_RenderCopy(renderer, exit_buttonTexture, NULL, &button_exit_rect);
			if (is_button_hit(button_record_rect, mx, my)) SDL_RenderCopy(renderer, button_recordTexture, NULL, &button_record_rect);
		}
		else {
			SDL_RenderCopy(renderer, confirmation_quit, NULL, &confirmation_quit_rect);
		}
		SDL_RenderPresent(renderer);

	}
}

void record(SDL_Renderer*& renderer) {
	TTF_Font* font = TTF_OpenFont("Text.ttf", 100);
	ifstream file;
	file.open("record.txt");
	int record_arr[12];
	for (int i = 0; i < 12; i++) {
		file >> record_arr[i];
	}
	SDL_Texture* texture;
	SDL_Event event;
	SDL_Rect backgroung_rect = { 0, 0, 1200, 700 };
	SDL_Texture* backgroundTexture = createIMG(renderer, "background\\record.jpg");
	SDL_Texture* exit_buttonTexture = createIMG(renderer, "button\\button_back.png");
	SDL_Rect button_back_rect = { 431, 540, 337, 79 };
	int k, num, mx, my;
	bool quit = false;
	while (1) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) exit(1);
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (is_button_hit(button_back_rect, event.button.x, event.button.y)) {
					SDL_DestroyTexture(backgroundTexture);
					SDL_DestroyTexture(exit_buttonTexture);
					TTF_CloseFont(font);
					return;
				}
			}
		}
		char text[10];
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroung_rect);
		for (int i = 0; i < 12; i++) {
			k = 0;
			_itoa_s(record_arr[i], text, 10);
			num = record_arr[i];
			if (num == 0) k = 1;
			while (num) {
				num /= 10;
				k++;
			}
			drawText(renderer, texture, text, font, { 1,1,1,1 }, { 255,255,255,0 }, { 693 + (4 - k) * 6,179 + 28 * i,11 * k,22 });
		}
		SDL_GetMouseState(&mx, &my);
		if (is_button_hit(button_back_rect, mx, my)) SDL_RenderCopy(renderer, exit_buttonTexture, NULL, &button_back_rect);
		SDL_RenderPresent(renderer);
	}
}

void level(SDL_Renderer*& renderer, bool& check_quit, int vol, int number_level, const char* branch1, const char* branch2, const char* branch3, const char* branch4, const char* branch5,
	const char* background_name, const char* name_level) {
	ifstream file_record;
	file_record.open("record.txt");
	int record_arr[12];
	for (int i = 0; i < 12; i++) {
		file_record >> record_arr[i];
	}
	TTF_Font* font = TTF_OpenFont("Text.ttf", 100);
	SDL_Texture* texture;
	srand(time(NULL));
	SDL_Texture* backgroundTexture = createIMG(renderer, background_name);
	SDL_Texture* pause = createIMG(renderer, "button\\pause.png");
	SDL_Texture* pause_window = createIMG(renderer, "background\\pause.png");
	SDL_Texture* complete_level = createIMG(renderer, "background\\comlete_level.png");;
	SDL_Texture** branch_arr = new SDL_Texture * [5];
	Mix_Chunk* Sound1 = Mix_LoadWAV("music\\branch.mp3");
	Mix_Chunk* Sound2 = Mix_LoadWAV("music\\win.mp3");
	branch_arr[0] = createIMG(renderer, branch1);
	branch_arr[1] = createIMG(renderer, branch2);
	branch_arr[2] = createIMG(renderer, branch3);
	branch_arr[3] = createIMG(renderer, branch4);
	branch_arr[4] = createIMG(renderer, branch5);
	ifstream file;
	file.open(name_level);
	int arr_el[100][5], arr_el2[100][5];
	int i = 0, flip_el, el, score = 10000, kol = 0;
	while (file) {
		;		for (int j = 0; j < 5; j++) file >> arr_el[i][j];
		i++;
	}
	i--;
	for (int j = 0; j < i; j++) {
		for (int b = 0; b < 4; b++) {
			arr_el2[j][b] = arr_el[j][b];
		}
		arr_el2[j][4] = (rand() % 4) * 90;
	}
	SDL_Rect backgroung_rect = { 0, 0, 1200, 700 }, branch1_rect = { 350,120,100,100 }, branch2_rect = { 450,120,100,100 },
		pause_rect = { 19,19,85,85 }, pause_window_rect = { 363, 29, 476, 638 }, complete_level_rect = { 362, 174, 449, 336 };
	bool quit = false, isFlip = false, isQuit = false, isComplete = false;
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
	SDL_Event event;
	while (1) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) exit(1);
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (!isComplete) {
					if (!isQuit) {
						if (is_ball_hit(pause_rect, event.button.x, event.button.y)) isQuit = true;
						if (!isFlip) {
							for (int j = 0; j < i; j++) {
								SDL_Rect branch = { arr_el2[j][0], arr_el2[j][1], arr_el2[j][2], arr_el2[j][2] };
								if (is_button_hit(branch, event.button.x, event.button.y)) {
									sound(Sound1);
									Mix_VolumeChunk(Sound1, int((1 - (double(772 - vol) / (772 - 436))) * 128));
									isFlip = true;
									flip_el = 0;
									el = j;
									if (score) score -= 100;
								}
							}
						}
					}
					else {
						if (is_ball_hit(pause_rect, event.button.x, event.button.y)) isQuit = false;
						if (is_button_hit({ 435, 198, 337, 79 }, event.button.x, event.button.y)) {
							SDL_DestroyTexture(backgroundTexture);
							SDL_DestroyTexture(pause);
							SDL_DestroyTexture(pause_window);
							SDL_DestroyTexture(complete_level);
							for (int vc = 0; vc < 5; vc++) {
								SDL_DestroyTexture(branch_arr[0]);
							}
							TTF_CloseFont(font);
							Mix_FreeChunk(Sound1);
							Mix_FreeChunk(Sound2);
							return;
						}
						if (is_button_hit({ 433, 318, 337, 79 }, event.button.x, event.button.y)) {
							check_quit = true;
							SDL_DestroyTexture(backgroundTexture);
							SDL_DestroyTexture(pause);
							SDL_DestroyTexture(pause_window);
							SDL_DestroyTexture(complete_level);
							for (int vc = 0; vc < 5; vc++) {
								SDL_DestroyTexture(branch_arr[0]);
							}
							TTF_CloseFont(font);
							Mix_FreeChunk(Sound1);
							Mix_FreeChunk(Sound2);
							return;
						}
						if (is_button_hit({ 435, 446, 337, 79 }, event.button.x, event.button.y)) {
							SDL_DestroyTexture(backgroundTexture);
							SDL_DestroyTexture(pause);
							SDL_DestroyTexture(pause_window);
							SDL_DestroyTexture(complete_level);
							for (int vc = 0; vc < 5; vc++) {
								SDL_DestroyTexture(branch_arr[0]);
							}
							TTF_CloseFont(font);
							Mix_FreeChunk(Sound1);
							Mix_FreeChunk(Sound2);
							exit(1);
						}
					}
				}
				else if (is_button_hit({ 434, 360, 337, 79 }, event.button.x, event.button.y)) {
					SDL_DestroyTexture(backgroundTexture);
					SDL_DestroyTexture(pause);
					SDL_DestroyTexture(pause_window);
					SDL_DestroyTexture(complete_level);
					for (int vc = 0; vc < 5; vc++) {
						SDL_DestroyTexture(branch_arr[0]);
					}
					TTF_CloseFont(font);
					Mix_FreeChunk(Sound1);
					Mix_FreeChunk(Sound2);
					return;
				}
			}
			if (!isComplete) {
				if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) {
					isQuit = true;
				}
			}
		}
		if (!isComplete) {
			if (!isQuit) {
				if (isFlip) {
					if (flip_el != 90) {
						arr_el2[el][4] = (arr_el2[el][4] + 1) % 360;
						SDL_Delay(2);
						flip_el++;
					}
					else isFlip = false;
				}
				int k = 0;
				for (int j = 0; j < i; j++) {
					if (arr_el2[j][3] == 0) k++;
					if (arr_el2[j][3] == 3) if ((arr_el2[j][4] == arr_el[j][4]) || (arr_el2[j][4] == arr_el[j][4] + 180) || (arr_el2[j][4] == arr_el[j][4] - 180)) k++;
					if ((arr_el2[j][3] == 1) || (arr_el2[j][3] == 2) || (arr_el2[j][3] == 4)) if (arr_el2[j][4] == arr_el[j][4]) k++;
				}
				if (k == i) {
					isComplete = true;
					sound(Sound2);
					Mix_VolumeChunk(Sound2, int((1 - (double(772 - vol) / (772 - 436))) * 128));
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroung_rect);
		for (int j = 0; j < i; j++) {
			SDL_Rect branch = { arr_el2[j][0], arr_el2[j][1], arr_el2[j][2], arr_el2[j][2] };
			SDL_RenderCopyEx(renderer, branch_arr[arr_el2[j][3]], NULL, &branch, arr_el2[j][4], NULL, flip);
		}
		char score_text[10];
		_itoa_s(score, score_text, 10);
		int num = score;
		kol = 0;
		while (num) {
			num /= 10;
			kol++;
		}
		if (score == 0) kol = 1;
		drawText(renderer, texture, score_text, font, { 1,1,1,1 }, { 255,255,255,0 }, { 615, 32, 20 * kol, 41 });
		SDL_RenderCopy(renderer, pause, NULL, &pause_rect);
		if (isQuit) SDL_RenderCopy(renderer, pause_window, NULL, &pause_window_rect);
		if (isComplete) {
			if (score > record_arr[number_level - 1]) ÑhangeLineFile(to_string(score), "record.txt", number_level);
			SDL_RenderCopy(renderer, complete_level, NULL, &complete_level_rect);
		}
		SDL_RenderPresent(renderer);
	}
}

void choose_level(SDL_Renderer*& renderer, int vol) {
	SDL_Event event;
	SDL_Rect backgroung_rect = { 0, 0, 1200, 700 }, back_rect = { 19,19,85,85 },
		rect1 = { 111,245,206,208 }, rect2 = { 371,245,206,208 }, rect3 = { 632,247,204,206 }, rect4 = { 884,247,204,206 },
		rect5 = { 112,245,205,208 }, rect6 = { 372,245,205,208 }, rect7 = { 633,247,203,206 }, rect8 = { 885,247,203,206 },
		rect9 = { 111,245,206,208 }, rect10 = { 372,245,206,208 }, rect11 = { 632,248,204,206 }, rect12 = { 885,248,204,206 };
	SDL_Texture* backgroundTextureBeach = createIMG(renderer, "background\\choose_level_branch.jpg");
	SDL_Texture* backgroundTexturePipes = createIMG(renderer, "background\\choose_level_pipes.jpg");
	SDL_Texture* backgroundTextureWires = createIMG(renderer, "background\\choose_level_wires.jpg");
	SDL_Texture* backTexture = createIMG(renderer, "button\\back_choose_menu.png");

	SDL_Texture* Texture1 = createIMG(renderer, "button_chose_level\\1.png");
	SDL_Texture* Texture2 = createIMG(renderer, "button_chose_level\\2.png");
	SDL_Texture* Texture3 = createIMG(renderer, "button_chose_level\\3.png");
	SDL_Texture* Texture4 = createIMG(renderer, "button_chose_level\\4.png");
	SDL_Texture* Texture5 = createIMG(renderer, "button_chose_level\\5.png");
	SDL_Texture* Texture6 = createIMG(renderer, "button_chose_level\\6.png");
	SDL_Texture* Texture7 = createIMG(renderer, "button_chose_level\\7.png");
	SDL_Texture* Texture8 = createIMG(renderer, "button_chose_level\\8.png");
	SDL_Texture* Texture9 = createIMG(renderer, "button_chose_level\\9.png");
	SDL_Texture* Texture10 = createIMG(renderer, "button_chose_level\\10.png");
	SDL_Texture* Texture11 = createIMG(renderer, "button_chose_level\\11.png");
	SDL_Texture* Texture12 = createIMG(renderer, "button_chose_level\\12.png");

	SDL_Texture* buttonBackTexture = createIMG(renderer, "button_chose_level\\button_back.png");
	int number_menu = 0, mx, my;
	bool quit = false, check_quit = false;
	while (1) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) exit(1);
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (abs(number_menu) % 3 == 0) {
					if (is_button_hit(rect1, event.button.x, event.button.y)) level(renderer, check_quit, vol, 1, "part2\\branch1.png", "part2\\branch2.png", "part2\\branch3.png",
						"part2\\branch4.png", "part2\\branch5.png", "background\\level_breanch.jpg", "level\\level1.txt");
					if (is_button_hit(rect2, event.button.x, event.button.y)) level(renderer, check_quit, vol, 2, "part2\\branch1.png", "part2\\branch2.png", "part2\\branch3.png",
						"part2\\branch4.png", "part2\\branch5.png", "background\\level_breanch.jpg", "level\\level2.txt");
					if (is_button_hit(rect3, event.button.x, event.button.y)) level(renderer, check_quit, vol, 3, "part2\\branch1.png", "part2\\branch2.png", "part2\\branch3.png",
						"part2\\branch4.png", "part2\\branch5.png", "background\\level_breanch.jpg", "level\\level3.txt");
					if (is_button_hit(rect4, event.button.x, event.button.y)) level(renderer, check_quit, vol, 4, "part2\\branch1.png", "part2\\branch2.png", "part2\\branch3.png",
						"part2\\branch4.png", "part2\\branch5.png", "background\\level_breanch.jpg", "level\\level4.txt");
				}
				if (abs(number_menu) % 3 == 1) {
					if (is_button_hit(rect5, event.button.x, event.button.y)) level(renderer, check_quit, vol, 5, "part\\branch1.png", "part\\branch2.png", "part\\branch3.png",
						"part\\branch4.png", "part\\branch5.png", "background\\level_pipes.jpg", "level\\level5.txt");
					if (is_button_hit(rect6, event.button.x, event.button.y)) level(renderer, check_quit, vol, 6, "part\\branch1.png", "part\\branch2.png", "part\\branch3.png",
						"part\\branch4.png", "part\\branch5.png", "background\\level_pipes.jpg", "level\\level6.txt");
					if (is_button_hit(rect7, event.button.x, event.button.y)) level(renderer, check_quit, vol, 7, "part\\branch1.png", "part\\branch2.png", "part\\branch3.png",
						"part\\branch4.png", "part\\branch5.png", "background\\level_pipes.jpg", "level\\level7.txt");
					if (is_button_hit(rect8, event.button.x, event.button.y)) level(renderer, check_quit, vol, 8, "part\\branch1.png", "part\\branch2.png", "part\\branch3.png",
						"part\\branch4.png", "part\\branch5.png", "background\\level_pipes.jpg", "level\\level8.txt");
				}
				if (abs(number_menu) % 3 == 2) {
					if (is_button_hit(rect9, event.button.x, event.button.y)) level(renderer, check_quit, vol, 9, "part3\\branch1.png", "part3\\branch2.png", "part3\\branch3.png",
						"part3\\branch4.png", "part3\\branch5.png", "background\\level_wires.jpg", "level\\level9.txt");
					if (is_button_hit(rect10, event.button.x, event.button.y)) level(renderer, check_quit, vol, 10, "part3\\branch1.png", "part3\\branch2.png", "part3\\branch3.png",
						"part3\\branch4.png", "part3\\branch5.png", "background\\level_wires.jpg", "level\\level10.txt");
					if (is_button_hit(rect11, event.button.x, event.button.y)) level(renderer, check_quit, vol, 11, "part3\\branch1.png", "part3\\branch2.png", "part3\\branch3.png",
						"part3\\branch4.png", "part3\\branch5.png", "background\\level_wires.jpg", "level\\level11.txt");
					if (is_button_hit(rect12, event.button.x, event.button.y)) level(renderer, check_quit, vol, 12, "part3\\branch1.png", "part3\\branch2.png", "part3\\branch3.png",
						"part3\\branch4.png", "part3\\branch5.png", "background\\level_wires.jpg", "level\\level12.txt");
				}
				if (is_triangle_hit(1117, 286, 1117, 396, 1184, 340, event.button.x, event.button.y)) number_menu++;
				if (is_triangle_hit(76, 287, 14, 340, 75, 397, event.button.x, event.button.y)) number_menu += 2;
				if (is_ball_hit(back_rect, event.button.x, event.button.y)) {
					SDL_DestroyTexture(backgroundTextureBeach);
					SDL_DestroyTexture(backgroundTexturePipes);
					SDL_DestroyTexture(backgroundTextureWires);
					SDL_DestroyTexture(backTexture);
					SDL_DestroyTexture(Texture1);
					SDL_DestroyTexture(Texture2);
					SDL_DestroyTexture(Texture3);
					SDL_DestroyTexture(Texture4);
					SDL_DestroyTexture(Texture5);
					SDL_DestroyTexture(Texture6);
					SDL_DestroyTexture(Texture7);
					SDL_DestroyTexture(Texture8);
					SDL_DestroyTexture(Texture9);
					SDL_DestroyTexture(Texture10);
					SDL_DestroyTexture(Texture11);
					SDL_DestroyTexture(Texture12);
					return;
				}
			}
		}
		if (check_quit) {
			SDL_DestroyTexture(backgroundTextureBeach);
			SDL_DestroyTexture(backgroundTexturePipes);
			SDL_DestroyTexture(backgroundTextureWires);
			SDL_DestroyTexture(backTexture);
			SDL_DestroyTexture(Texture1);
			SDL_DestroyTexture(Texture2);
			SDL_DestroyTexture(Texture3);
			SDL_DestroyTexture(Texture4);
			SDL_DestroyTexture(Texture5);
			SDL_DestroyTexture(Texture6);
			SDL_DestroyTexture(Texture7);
			SDL_DestroyTexture(Texture8);
			SDL_DestroyTexture(Texture9);
			SDL_DestroyTexture(Texture10);
			SDL_DestroyTexture(Texture11);
			SDL_DestroyTexture(Texture12);
			return;
		}
		SDL_RenderClear(renderer);
		SDL_GetMouseState(&mx, &my);
		switch (abs(number_menu) % 3) {
		case 0:
			SDL_RenderCopy(renderer, backgroundTextureBeach, NULL, &backgroung_rect);
			if (is_button_hit(rect1, mx, my)) SDL_RenderCopy(renderer, Texture1, NULL, &rect1);
			if (is_button_hit(rect2, mx, my)) SDL_RenderCopy(renderer, Texture2, NULL, &rect2);
			if (is_button_hit(rect3, mx, my)) SDL_RenderCopy(renderer, Texture3, NULL, &rect3);
			if (is_button_hit(rect4, mx, my)) SDL_RenderCopy(renderer, Texture4, NULL, &rect4);
			break;
		case 1:SDL_RenderCopy(renderer, backgroundTexturePipes, NULL, &backgroung_rect);
			if (is_button_hit(rect1, mx, my)) SDL_RenderCopy(renderer, Texture5, NULL, &rect5);
			if (is_button_hit(rect2, mx, my)) SDL_RenderCopy(renderer, Texture6, NULL, &rect6);
			if (is_button_hit(rect3, mx, my)) SDL_RenderCopy(renderer, Texture7, NULL, &rect7);
			if (is_button_hit(rect4, mx, my)) SDL_RenderCopy(renderer, Texture8, NULL, &rect8);
			break;
		case 2:SDL_RenderCopy(renderer, backgroundTextureWires, NULL, &backgroung_rect);
			if (is_button_hit(rect1, mx, my)) SDL_RenderCopy(renderer, Texture9, NULL, &rect9);
			if (is_button_hit(rect2, mx, my)) SDL_RenderCopy(renderer, Texture10, NULL, &rect10);
			if (is_button_hit(rect3, mx, my)) SDL_RenderCopy(renderer, Texture11, NULL, &rect11);
			if (is_button_hit(rect4, mx, my)) SDL_RenderCopy(renderer, Texture12, NULL, &rect12);
			break;
		}
		SDL_RenderCopy(renderer, backTexture, NULL, &back_rect);
		if (is_ball_hit(back_rect, mx, my)) SDL_RenderCopy(renderer, buttonBackTexture, NULL, &back_rect);
		SDL_RenderPresent(renderer);
	}
}