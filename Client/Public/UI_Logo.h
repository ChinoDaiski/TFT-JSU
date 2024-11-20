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

class CUI_Logo final : public CGameObject
{
private:
	explicit CUI_Logo(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CUI_Logo(const CUI_Logo& rhs);
	virtual ~CUI_Logo() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	const vector<pair<const _tchar*, CUI*>>& Get_vUI(void) const { return m_vUI; }

private:
	vector<pair<const _tchar*, CUI*>> m_vUI;

private:
	HRESULT Set_UI();

public:
	static CUI_Logo* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END