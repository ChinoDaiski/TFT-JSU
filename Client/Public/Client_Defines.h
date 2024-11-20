#pragma once

namespace Client
{
	const unsigned int	g_iWinCX = 1280;
	const unsigned int	g_iWinCY = 720;

}

#include <process.h>
#include "Client_Struct.h"




#define  LAYER_LOGO_CAMERA					TEXT("Layer_Logo_Camera")
#define  LAYER_LOGO_MAP						TEXT("Layer_Logo_MAP")
#define  LAYER_LOGO_UI						TEXT("Layer_Logo_UI")
#define  LAYER_LOGO_PLAYER					TEXT("Layer_Logo_PLAYER")
#define  LAYER_LOGO_EFFECT					TEXT("Layer_Logo_EFFECT")

#define  LAYER_LOADING_CAMERA				TEXT("Layer_Loading_Camera")
#define  LAYER_LOADING_MAP					TEXT("Layer_Loading_MAP")
#define  LAYER_LOADING_UI					TEXT("Layer_Loading_UI")
#define  LAYER_LOADING_PLAYER				TEXT("Layer_Loading_PLAYER")
#define  LAYER_LOADING_EFFECT				TEXT("Layer_Loading_EFFECT")

#define  LAYER_GAMEPLAY_CAMERA				TEXT("Layer_GamePlay_Camera")
#define  LAYER_GAMEPLAY_MAP					TEXT("Layer_GamePlay_MAP")
#define  LAYER_GAMEPLAY_MAP_CENTER			TEXT("Layer_GamePlay_MAP_CENTER")
#define  LAYER_GAMEPLAY_MAP_PEROSNAL		TEXT("Layer_GamePlay_MAP_PERSONAL")
#define  LAYER_GAMEPLAY_HEXACORE			TEXT("Layer_GamePlay_Hexacore")
#define  LAYER_GAMEPLAY_UI					TEXT("Layer_GamePlay_UI")
#define  LAYER_GAMEPLAY_PLAYER				TEXT("Layer_GamePlay_PLAYER")
#define  LAYER_GAMEPLAY_EFFECT				TEXT("Layer_GamePlay_EFFECT")
#define  LAYER_GAMEPLAY_MARKET				TEXT("Layer_GamePlay_MARKET")
#define  LAYER_GAMEPLAY_CHAMPION			TEXT("Layer_GamePlay_Champion")
#define  LAYER_GAMEPLAY_ITEM				TEXT("Layer_GamePlay_Item")

#define  LAYER_GAMEPLAY_TILE_CENTERMAP				TEXT("Layer_GamePlay_Tile_CenterMap")
#define  LAYER_GAMEPLAY_TILE_PERSONAL01_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap01_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL02_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap02_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL03_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap03_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL04_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap04_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL05_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap05_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL06_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap06_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL07_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap07_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL08_FIELD		TEXT("Layer_GamePlay_Tile_PersonalMap08_Field")
#define  LAYER_GAMEPLAY_TILE_PERSONAL01_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap01_Lounge")
#define  LAYER_GAMEPLAY_TILE_PERSONAL02_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap02_Lounge")
#define  LAYER_GAMEPLAY_TILE_PERSONAL03_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap03_Lounge")
#define  LAYER_GAMEPLAY_TILE_PERSONAL04_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap04_Lounge")
#define  LAYER_GAMEPLAY_TILE_PERSONAL05_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap05_Lounge")
#define  LAYER_GAMEPLAY_TILE_PERSONAL06_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap06_Lounge")
#define  LAYER_GAMEPLAY_TILE_PERSONAL07_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap07_Lounge")
#define  LAYER_GAMEPLAY_TILE_PERSONAL08_LOUNGE		TEXT("Layer_GamePlay_Tile_PersonalMap08_Lounge")

#define  LAYER_GAMEPLAY_BUTTON_Map			TEXT("Layer_GamePlay_Button_Map")
#define  LAYER_GAMEPLAY_BUTTON_Champion		TEXT("Layer_GamePlay_Button_Champion")
#define  LAYER_GAMEPLAY_BUTTON_LockUnlock	TEXT("Layer_GamePlay_Button_LockUnlock")
#define  LAYER_GAMEPLAY_BUTTON_LevelUp		TEXT("Layer_GamePlay_Button_LevelUp")
#define  LAYER_GAMEPLAY_BUTTON_Reroll		TEXT("Layer_GamePlay_Button_Reroll")

#define	 LAYER_GAMEPLAY_PICKING_OBJECT		TEXT("Layer_GamePlay_Picking_Object")

#define	 LAYER_GAMEPLAY_MOUSE				TEXT("Layer_GamePlay_Mouse")

#define  LAYER_ENDING_CAMERA				TEXT("Layer_Ending_Camera")
#define  LAYER_ENDING_MAP					TEXT("Layer_Ending_MAP")
#define  LAYER_ENDING_UI					TEXT("Layer_Ending_UI")
#define  LAYER_ENDING_PLAYER				TEXT("Layer_Ending_PLAYER")
#define  LAYER_ENDING_EFFECT				TEXT("Layer_Ending_EFFECT")

#define	 MAP_GAP 25.f


extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
