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

class CPickingObject : public CGameObject {
public:

	typedef struct _tagPickingDesc {
		_float3		vPos;			// 사각형의 중점
		_float2		fSize;			// 사각형의 가로/세로 길이
	}PICKINGDESC;

protected:
	explicit CPickingObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPickingObject(const CPickingObject& rhs);
	virtual ~CPickingObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

public:
	_float3*	Get_RectPos(void) { return m_RectPos; }

private:
	_float3				m_RectPos[4];	// 피킹을 위한 점 4개의 정보를 담고 있는 배열, 각 0,1,2 / 0,2,3 의 순서로 삼각형을 만든다.

public:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CPickingObject* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END