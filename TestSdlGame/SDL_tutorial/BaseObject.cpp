#include "BaseObject.h"

using namespace std;


BaseObject::BaseObject()
{
	p_object == NULL;
	rect_object.x = 0;
	rect_object.y = 0;
	rect_object.w = 0;
	rect_object.h = 0;

}

BaseObject::~BaseObject()
{
	Free();
}

bool BaseObject::LoadImg(const char* path, SDL_Renderer* screen) {
	SDL_Texture* new_texture = NULL;

	SDL_Surface* load_surface = IMG_Load(path);
	if (load_surface == NULL) { cout << " FAIL TO LOAD SURFACE !!" << endl; }
	else {
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, color_key_r, color_key_g, color_key_b));
		// Color key dung de xoa di phan background trong anh cua nhan vat

		new_texture = SDL_CreateTextureFromSurface(screen, load_surface); // Dung de chuyen surface thanh texture

		if (new_texture == NULL) { cout << "Fail to convert surface into texture !!"; }
		else {
			rect_object.w = load_surface->w;
			rect_object.h = load_surface->h;
			// dung  de gan thong so cua tam anh vao BaseObject
		}

		SDL_FreeSurface(load_surface); // xoa surface vi thong so cua surface da chuyen sang new_texture
	}

	p_object = new_texture;

	if (p_object != NULL) { return true; }
	else { return false; }  // co the thay ca cau lenh if else tren bang cau lenh : return p_object != NULL;
}

// thong so cua new_texture da duoc gan cho p_object,sau ham LoadImg, doi tuong thuoc class BaseObject da co thong tin ve kich co va texture cua tam anh

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip /*= NULL */) {
	SDL_Rect renderquad = { rect_object.x, rect_object.y, rect_object.w, rect_object.h };
	// bao gom thong tin ve vi tri tam anh va kich thuoc cua tam anh

	SDL_RenderCopy(des, p_object, clip, &renderquad); // day toan bo thong so cua p_object vao des voi kich thuoc va vi tri da luu trong bien rederquad

}

void BaseObject::Free()
{
	if (p_object != NULL)
	{
		SDL_DestroyTexture(p_object);
		p_object = NULL;
		rect_object.w = 0;
		rect_object.h = 0;
	}
}


