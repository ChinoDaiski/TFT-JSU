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
	// UI�� ����
	// HIDE�� �÷��̾ ���� ����� ���� ��, UI�� ���ܾ� �ϰ�, NONHIDE�� ������� �׷�����.
	// �̴ϸʰ� �������� â�� ��������� ������ UI�� �÷��̾ �� ��� ������ ���������Ѵ�.
	enum UI_TYPE { UT_HIDE, UT_NONHIDE, UT_END };

public:
	typedef struct _tagUIDesc {
		_tchar	_tagTextureName[MAX_PATH];	// �̹��� �±�

		_float	fX, fY;					// �̹����� ����
		_float	fSizeX, fSizeY;			// �̹����� ����, ���� ������

		_uint	iTextureNum;			// �̹��� ��ȣ

		UI_TYPE	eType;					// UI�� Ÿ��
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
	_float4x4			m_ProjMatrix;	// �������� ���
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