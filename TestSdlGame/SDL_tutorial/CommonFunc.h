#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include<windows.h>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

using namespace std;


static SDL_Window *g_window = NULL; // build a window form for game
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event; // event accept mouse, keyboard

//SCREEN

const int screen_width = 1280;
const int screen_height = 720;
const int screen_BPP = 32; // chi so ve pixel

const int color_key_r = 167;
const int color_key_g = 175;
const int color_key_b = 180;



#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 15


const int FrameSize = 8;
typedef struct Input // cau truc danh cho viec di chuyen
{
	int left;
	int right;
	int up;
	int down;
	int jump;
};



typedef struct Map
{
	int start_x;
	int start_y;

	int max_x;
	int max_y;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	const char* file_name;
};

#endif // COMMON_FUNCTION_H
