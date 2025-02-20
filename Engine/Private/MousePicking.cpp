#include "..\Public\MousePicking.h"

IMPLEMENT_SINGLETON(CMousePicking)

CMousePicking::CMousePicking()
{
}

HRESULT CMousePicking::InitPicking(HWND hWnd, _uint iWinCX, _uint iWinCY)
{
	m_hWnd = hWnd;
	m_iWinCX = iWinCX;
	m_iWinCY = iWinCY;

	return S_OK;
}

void CMousePicking::Make_Ray(void)
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	POINT pt = Get_Mouse();
	_vector vMousePos = { pt.x / (m_iWinCX / 2.f) - 1 , pt.y / (m_iWinCY / -2.f) + 1 , 0.f, 1.f };

	vMousePos = XMVector3TransformCoord(vMousePos, XMMatrixInverse(nullptr, pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ)));
	XMStoreFloat3(&m_vRay, vMousePos);
	XMStoreFloat3(&m_vRayPos, _vector());

	XMStoreFloat3(&m_vRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), XMMatrixInverse(nullptr, pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat3(&m_vRay, XMVector3TransformNormal(XMLoadFloat3(&m_vRay), XMMatrixInverse(nullptr, pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat3(&m_vRay, XMVector3Normalize(XMLoadFloat3(&m_vRay)));

	RELEASE_INSTANCE(CPipeLine);
}

_bool CMousePicking::Picking(_float3* pVerticesPos, _fmatrix WorldMatrix, _vector* pOut)
{
	Make_Ray();

	_vector vRay = XMLoadFloat3(&m_vRay);
	XMVectorSetW(vRay, 0.f);
	_vector vRayPos = XMLoadFloat3(&m_vRayPos);
	XMVectorSetW(vRayPos, 1.f);

	vRayPos = XMVector3TransformCoord(vRayPos, XMMatrixInverse(nullptr, WorldMatrix));
	vRay = XMVector3TransformNormal(vRay, XMMatrixInverse(nullptr, WorldMatrix));
	vRay = XMVector3Normalize(vRay);

	_vector v1 = XMLoadFloat3(&pVerticesPos[0]);
	_vector v2 = XMLoadFloat3(&pVerticesPos[1]);
	_vector v3 = XMLoadFloat3(&pVerticesPos[2]);

	_float fDistance = 0.f;
	if (TriangleTests::Intersects(vRayPos, vRay, v1, v2, v3, fDistance))
	{
		_vector vOut = { XMVectorGetX(vRayPos + vRay * fDistance), XMVectorGetY(vRayPos + vRay * fDistance), XMVectorGetZ(vRayPos + vRay * fDistance), fDistance };
		*pOut = XMVector3TransformCoord(vOut, WorldMatrix);
		return true;
	}
	return false;
}

const CMousePicking::RAYDESC CMousePicking::Get_vRayDesc(void)
{
	Make_Ray();

	RAYDESC RayDesc;
	RayDesc.vRay = m_vRay;
	RayDesc.vRayPos = m_vRayPos;

	return RayDesc;
}

POINT CMousePicking::Get_Mouse(void)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	return ptMouse;
}

void CMousePicking::Free()
{
}