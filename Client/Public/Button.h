#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END



/*
	버튼은 눌리면 true 아니면 false, 마우스를 올려놓는것과는 상관이없다.
	버튼은


*/

BEGIN(Client)

class CButton : public CGameObject {
public:
	enum BUTTON_TYPE {
		BT_LOCK_UNLCOK,		// 잠금, 잠금 해제 - 상점창을 잠그는 버튼
		BT_MINIMAP,			// 미니맵 - 연결된 위치로 카메라를 이동하는 버튼
		BT_CHAMPION,		// 챔피언 구매 버튼 - 원하는 챔피언을 구매하는 버튼
		BT_LEVELUP,			// 레벨업 버튼 - 레벨업을 하는 버튼
		BT_REROLL,			// 리롤 버튼 - 상점창을 새로고침하는 버튼

		BT_END
	};

protected:
	explicit CButton(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CButton(const CButton& rhs);
	virtual ~CButton() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CTexture*			m_pTextureCom = nullptr;
	CTexture*			m_pTextureComBorder = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

protected:
	_float4x4			m_ProjMatrix;

protected:
	virtual void Click(void);

private:
	void CheckClick(void);

protected:
	RECT				m_rtInfo;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	HRESULT SetUp_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

public:
	static CButton* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	BUTTON_TYPE		Get_Type(void) { return m_eType; }

protected:
	BUTTON_TYPE		m_eType;
};

END