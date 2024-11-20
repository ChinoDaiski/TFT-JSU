#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Engine)

class CUI final : public CGameObject {
public:
	typedef struct _tagUIDesc {
		_tchar	_tagTextureName[MAX_PATH];	// 이미지 태그

		_float	fX, fY;					// 이미지의 중점
		_float	fSizeX, fSizeY;			// 이미지의 가로, 세로 사이즈

		_uint		iLevel;
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
	int m_iWinCX = 0;
	int m_iWinCY = 0;

public:
	HRESULT SetUp_Components(void * pArg);

public:
	static CUI* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, int g_iWinCX, int g_iWinCY);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void Set_PriorityNumber(int iNum) { m_iPriorityNumber = iNum; }
	_uint Get_PriorityNumber(void) const { return m_iPriorityNumber; }

private:
	_uint	m_iPriorityNumber = 0;
};

END