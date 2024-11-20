#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "PersonalMap.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	explicit CLevel_GamePlay(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_CenterMap(const _tchar* pLayerTag);
	HRESULT Ready_Layer_PersonalMap(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Hexacore(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Market(const _tchar* pLayerTag);
	HRESULT Ready_Layer_PickingObject(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Tile();
	HRESULT Ready_Layer_Tile_PersonalMap01();
	HRESULT Ready_Layer_Tile_PersonalMap02();
	HRESULT Ready_Layer_Tile_PersonalMap03();
	HRESULT Ready_Layer_Tile_PersonalMap04();
	HRESULT Ready_Layer_Tile_PersonalMap05();
	HRESULT Ready_Layer_Tile_PersonalMap06();
	HRESULT Ready_Layer_Tile_PersonalMap07();
	HRESULT Ready_Layer_Tile_PersonalMap08();
	HRESULT Ready_PushTileInfo(const _tchar* pLayerTagField, const _tchar* pLayerTagLounge, CPersonalMap::MAP_NAME eName);


	HRESULT Ready_Layer_Button_Map(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Button_Champion(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Button_LevelUp(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Button_Reroll(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Button_LockUnlock(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Champion(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Champion_Cost1(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Champion_Cost2(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Champion_Cost3(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Champion_Cost4(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Champion_Cost5(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Mouse(const _tchar* pLayerTag);

private:
	HRESULT Create_FieldTile(_vector vPos, _float2 fScale, const _tchar* pLayerTag);
	HRESULT Create_LoungeTile(_vector vPos, _float2 fScale, const _tchar* pLayerTag);

private:
	CTransform* Find_PersonalMapTransform(CPersonalMap::MAP_NAME eName);
	CTransform* Find_CenterMapTransform(void);

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual void Free() override;
};

END