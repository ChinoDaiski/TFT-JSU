#pragma once

#include "Base.h"
#include "UI.h"

BEGIN(Engine)

class CUI_Manager : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

public:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);		// ���̾� �Ҵ�
	HRESULT Add_UIObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, _uint iLevelIndex, const _tchar* pTagUI);	// ���ϴ� ������ UI ������Ʈ �߰�
	void Tick(_double TimeDelta);		// UI ���� ����
	void LateTick(_double TimeDelta);
	HRESULT Clear(_uint iLevelIndex);

private:
	vector<pair<const _tchar*, CUI*>>*			m_vUI = nullptr;
	typedef vector<pair<const _tchar*, CUI*>>	VUI;

private:
	_uint			m_iNumLevels = 0;	// ������ ����

public:
	virtual void Free() override;
};

END