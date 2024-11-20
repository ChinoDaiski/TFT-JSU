#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI final : public CGameObject {
public:
	// UI의 종류
	// HIDE는 플레이어가 맵의 가운데에 있을 때, UI를 숨겨야 하고, NONHIDE면 상관없이 그려진다.
	// 미니맵과 스테이지 창은 상관없지만 나머지 UI는 플레이어가 맵 가운데 있으면 숨겨져야한다.
	enum UI_TYPE { UT_HIDE, UT_NONHIDE, UT_END };

public:
	typedef struct _tagUIDesc {
		_tchar	_tagTextureName[MAX_PATH];	// 이미지 태그

		_float	fX, fY;					// 이미지의 중점
		_float	fSizeX, fSizeY;			// 이미지의 가로, 세로 사이즈

		_uint	iTextureNum;			// 이미지 번호

		UI_TYPE	eType;					// UI의 타입
	} UIDesc;

private:
	explicit CUI(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;	// 직교투영 행렬
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	HRESULT SetUp_Components(void * pArg);

public:
	static CUI* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void Set_PriorityNumber(int iNum) { m_iPriorityNumber = iNum; }
	_uint Get_PriorityNumber(void) const { return m_iPriorityNumber; }

public:
	UI_TYPE Get_Type(void) { return m_eType; }

private:
	_uint	m_iPriorityNumber = 0;
	UI_TYPE	m_eType = UT_END;
};

END