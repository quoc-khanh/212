#include "MainPlayer.h"
#include"CommonFunc.h"
#include"BaseObject.h"


#define Gravity 0.8
#define Max_Fall_Speed 10
#define Player_Speed  8


MainPlayer::MainPlayer()
{
	frame = 0;
	x_pos = 0;
	y_pos = 0;
	x_val = 0;
	y_val = 0;
	width_frame = 0;
	height_frame = 0;
	status = -1;
	Input_type.left = 0;
	Input_type.right = 0;
	Input_type.jump = 0;
	Input_type.up = 0;
	Input_type.down = 0;
	on_ground = false;
}

MainPlayer::~MainPlayer()
{

}

bool MainPlayer::LoadImg(const char* path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		width_frame = rect_object.w / FrameSize;
		height_frame = rect_object.h;
	}

	return ret;
}

void MainPlayer::set_clip()
{
	if (width_frame > 0 && height_frame > 0)
	{
		for (int i = 0; i < FrameSize; i++)
		{
			frame_clip[i].x = i * width_frame;
			frame_clip[i].y = 0;
			frame_clip[i].w = width_frame;
			frame_clip[i].h = height_frame;
		}
	}
}

void MainPlayer::Show(SDL_Renderer* des)
{
	if (status == walk_left)
	{
		LoadImg("Pic/player_left.png", des);
	}
	else
	{
		LoadImg("Pic/player_right.png", des);
	}

	if (Input_type.left == 1 || Input_type.right == 1)
	{
		frame++;
	}
	else frame = 0;

	if (frame >= 8)
	{
		frame = 0;
	}

	rect_object.x = x_pos;
	rect_object.y = y_pos;

	SDL_Rect* current_clip = &frame_clip[frame];

	SDL_Rect renderQuad = { rect_object.x, rect_object.y, width_frame, height_frame }; // chua toa do dang o va kich thuoc

	SDL_RenderCopy(des, p_object, current_clip, &renderQuad);
}



void MainPlayer::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status = Walk_right;
			Input_type.right = 1;
			Input_type.left = 0;
		}
		break;
		case SDLK_LEFT:
		{
			status = walk_left;
			Input_type.left = 1;
			Input_type.right = 0;
		}
		break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			Input_type.right = 0;
		}
		break;
		case SDLK_LEFT:
		{
			Input_type.left = 0;
		}
		break;
		}
	}
}

void MainPlayer::DoPlayer(Map& map_data)
{
	x_val = 0;
	y_val += Gravity; // toc do roi cua nhan vat

	if (y_val > Max_Fall_Speed) {
		y_val = Max_Fall_Speed;
	}

	if (Input_type.left == 1)
	{
		x_val -= Player_Speed;
	}
	else if (Input_type.right == 1)
	{
		x_val += Player_Speed;
	}

	CheckToMap(map_data);
}

void MainPlayer::CheckToMap(Map& map_data)
{
	int x1 = 0; // gioi han kiem tra theo chieu x
	int x2 = 0;

	int y1 = 0; // tuong tu nhu tren nhung doi voi y
	int y2 = 0;

	//check horizontal

	int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;
	x1 = (x_pos + x_val) / TILE_SIZE; // Xem xem o hien tai cua nhan vat dang la bao nhieu;
	x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;

	y1 = (y_pos) / TILE_SIZE;
	y2 = (y_pos + height_min - 1) / TILE_SIZE;

	/* giai thich
		x1,y1 ********x2,y1
		 *
		 *
		 *

	*/

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (x_val > 0) // doi tuong dang di sang phai
		{
			if (map_data.tile[y1][x2] != 0 || map_data.tile[y2][x2] != 0)
			{
				x_pos = x2 * TILE_SIZE; // gap va cham thi dung tai vi tri day;
				x_pos -= width_frame + 1;
				x_val = 0;
			}
		}
		else if (x_val < 0)
		{
			if (map_data.tile[y1][x1] != 0 || map_data.tile[y2][x1] != 0)
			{
				x_pos = (x1 + 1) *TILE_SIZE;
				x_val = 0;
			}
		}
	}

	//Check vertical

	int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
	x1 = (x_pos) / TILE_SIZE;
	x2 = (x_pos + width_min) / TILE_SIZE;

	y1 = (y_pos + y_val) / TILE_SIZE;
	y2 = (y_pos + y_val + height_frame - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val > 0)
		{
			if (map_data.tile[y2][x1] != 0 || map_data.tile[y2][x2] != 0)
			{
				y_pos = y2 * TILE_SIZE;
				y_pos -= (height_frame + 1);
				y_val = 0;
				on_ground = true;
			}
		}

		else if (y_val < 0)
		{
			if (map_data.tile[y1][x1] != 0 || map_data.tile[y1][x2] != 0)
			{
				y_pos = (y1 + 1)*TILE_SIZE;
				y_val = 0;
			}
		}
	}

	x_pos += x_val;
	y_pos += y_val;

	if (x_pos < 0) {
		x_pos = 0;
	}
	else if (x_pos + width_frame > map_data.max_x)
	{
		x_pos = map_data.max_x - width_frame - 1;
	}

}


