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

class CMouse : public CGameObject {
public:
	typedef struct _tagMouseDesc {
		_float2		fSize;			// 마우스 UI의 가로/세로 길이
	}MOUSEDESC;

protected:
	explicit CMouse(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CMouse(const CMouse& rhs);
	virtual ~CMouse() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

private:
	void Device_Input(void);	// 마우스를 누르면 발생하는 일들과 관련된 함수
	void Cal_MousePos(void);	// 마우스의 좌표를 매번 갱신해주는 함수
	void Move_PickingObject(void);	// 클릭해서 누른 오브젝트를 움직이는 함수
	void Check_Collision();		// 피킹된 오브젝트와 타일의 충돌 체크

private:
	// 피킹 관련 함수
	CGameObject*	Picking_ChampAndItem(void);	// 챔피언과 아이템을 피킹하면 해당 오브젝트 정보를 반환하는 함수

protected:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;	// 직교투영 행렬
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	POINT		Get_MousePos(void) { return m_ptMouse; }

private:
	POINT			m_ptMouse;	// 피킹을 위한 마우스의 실시간 위치 정보
	_bool			m_bPicking = false;		// 특정 오브젝트가 눌렸을 경우에만 true
	CGameObject*	m_pPickekdObject = nullptr;		// 선택된 오브젝트
	_vector			m_vPickingObjectOriginPos;		// 선택된 오브젝트의 원래 자리 위치
	CGameObject*	m_pCollisionTile = nullptr;		// 선택된 오브젝트와 충돌된 타일 정보

public:
	static CMouse* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END