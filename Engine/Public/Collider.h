#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		_float3			vPosition;
		_float3			vSize;
		_float			fRadius;
	}COLLIDERDESC;

	typedef struct tagObbDesc
	{
		_float3			vCenter;
		_float3			vAlignAxis[3];
		_float3			vCenterAxis[3];
	}OBBDESC;


public:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_bool Collsion_AABB(CCollider* pTargetCollider);
	_bool Collsion_OBB(CCollider* pTargetCollider);
	_bool Collsion_SPHERE(CCollider* pTargetCollider);
	void Update(_fmatrix WorldMatrix);

public:
	_bool Collsion_AABB(_fvector vRay, _fvector vRayPos, _float& fDist);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

public:
	_bool isCollision(void) { return m_isCollision; }

private:
	TYPE						m_eType = TYPE_END;
	_bool						m_isCollision = false;

	BoundingBox*				m_pAABB = nullptr;
	BoundingBox*				m_pAABBInWorld = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;
	BoundingOrientedBox*		m_pOBBInWorld = nullptr;
	BoundingSphere*				m_pSPHERE = nullptr;
	BoundingSphere*				m_pSPHEREInWorld = nullptr;

#ifdef _DEBUG
private:
	BasicEffect*								m_pBasicEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = nullptr;
	ID3D11InputLayout*							m_pInputLayout = nullptr;
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix);
	_vector Compute_Min();
	_vector Compute_Max();
	OBBDESC Compute_OBB();

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END