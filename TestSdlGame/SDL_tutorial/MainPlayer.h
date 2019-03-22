
#ifndef MAINPLAYER_H
#define MAINPLAYER_H

#include"CommonFunc.h"
#include"BaseObject.h"

class MainPlayer : public BaseObject
{
public:
	MainPlayer();
	~MainPlayer();

	enum WalkType
	{
		Walk_right = 0,
		walk_left = 1,

	};

	bool LoadImg(const char* path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des); // ham de show frame;
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen); // ham de nhan ban phim
	void set_clip(); // xu li ve animation

	void DoPlayer(Map& map_data); // ham de cong tru di luong cho nhan van di chuyen, can truyen vao ban do de check;
	void CheckToMap(Map& map_data);

	bool on_ground;


private:
	float x_val; // khi an sang phai sang trai mot luong x
	float y_val; // giong tren nhung la len xuong

	float x_pos;
	float y_pos;

	int width_frame; //luu tru kich thuoc cua nhan vat
	int height_frame;

	SDL_Rect frame_clip[FrameSize]; // kich thuoc so frame
	Input Input_type;
	int frame; // bien luu chi so o frame xemd ang o frame nao;
	int status; // bien luu trang thai di chuyen;


};


#endif // MAINPLAYER_H
