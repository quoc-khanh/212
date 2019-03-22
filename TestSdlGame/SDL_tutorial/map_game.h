
#ifndef MAP_GAME_H
#define MAP_GAME_H

#include"CommonFunc.h"
#include"BaseObject.h"

#define MAX_TILES 20

class TileMap : public BaseObject
{
public:
	TileMap() { ; }
	~TileMap() { ; }

};


class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	void LoadMap(const char* name);
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);
	Map GetMap() const { return game_map; } // do bien game_map la private, khong the tury cap nen goi mot ham tra lai gia tri cua bien game_map de dung.
private:
	Map game_map;
	TileMap tile_map[MAX_TILES];
};





#endif // MAP_GAME_H

