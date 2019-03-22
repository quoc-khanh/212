#include<SDL.h>
#include<SDL_image.h>
#include"CommonFunc.h"
#include"BaseObject.h"
#include"map_game.h"
#include"MainPlayer.h"

BaseObject g_background;
using namespace std;

bool InitData()
{
	bool result = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("Test Game Using SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	if (g_window == NULL)
	{
		result = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) result = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags)) // <==> Neu IMG_Init(imgFlags) == false && imgFlags == false
			{
				result = false;
			}

		}
	}

	return result;
}


bool Load_Background()
{
	bool Backgroundload = g_background.LoadImg("Pic/Background.png", g_screen);

	return Backgroundload;

}


void Close()  // ham tat tat ca cac chuong trinh di.
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();

}


int main(int argc, char* argv[]) {

	if (InitData() == false) return -1;
	if (Load_Background() == false) return -1;


	GameMap game_map;
	game_map.LoadMap("map/map01.dat");
	game_map.LoadTiles(g_screen);


	MainPlayer p_player;
	p_player.LoadImg("Pic/player_right.png", g_screen);
	p_player.set_clip();


	bool is_quit = false;

	while (is_quit == false)
	{
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
				cout << " Shut down" << endl;
			}

			p_player.HandleInputAction(g_event, g_screen);
		}


		SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);
		game_map.DrawMap(g_screen);
		Map MapData = game_map.GetMap();

		p_player.DoPlayer(MapData);
		p_player.Show(g_screen);


		SDL_RenderPresent(g_screen);


	}

	Close();


	return 0;
}
