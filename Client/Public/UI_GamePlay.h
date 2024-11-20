#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_GamePlay final : public CGameObject
{
private:
	explicit CUI_GamePlay(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CUI_GamePlay(const CUI_GamePlay& rhs);
	virtual ~CUI_GamePlay() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	vector<pair<const _tchar*, CUI*>> m_vUI;

private:
	HRESULT Set_UI(void);
	HRESULT Set_UI_Round(void);
	HRESULT Set_UI_MiniMap(void);
	HRESULT Set_UI_Market(void);
	HRESULT Set_UI_Players(void);
	HRESULT Set_UI_Synergy(void);

public:
	static CUI_GamePlay* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END