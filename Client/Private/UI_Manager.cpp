#include "..\Public\UI_Manager.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (0 != m_iNumLevels)
		return E_FAIL;

	m_vUI = new VUI[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CUI_Manager::Add_UIObject(ID3D11Device * pDeviceOut, ID3D11DeviceContext * pDeviceContextOut, _uint iLevelIndex, const _tchar * pTagUI)
{
	return S_OK;
}

void CUI_Manager::Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		// ������������ ����, ��ȣ�� �������� �ڷ� ��
		sort(m_vUI[i].begin(), m_vUI[i].end(), [](const pair<const _tchar*, CUI*>& a, const pair<const _tchar*, CUI*>& b) {
			return a.second->Get_PriorityNumber() > b.second->Get_PriorityNumber();
		});

		// �켱 ������ ���� UI(���� �տ� �׷����� �ϴ� UI)�ϼ��� ���߿� �׷���
		for (auto& Pair : m_vUI[i])
		{
			Pair.second->Tick(TimeDelta);

		}
	}
}

void CUI_Manager::LateTick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_vUI[i])
		{
			Pair.second->LateTick(TimeDelta);
		}
	}
}

HRESULT CUI_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_vUI[iLevelIndex])
		Safe_Release(Pair.second);

	m_vUI[iLevelIndex].clear();

	return S_OK;
}

void CUI_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_vUI[i])
			Safe_Release(Pair.second);
		m_vUI[i].clear();
	}

	Safe_Delete_Array(m_vUI);
}
