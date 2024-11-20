#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* �������� �ε��ؾ��� ���ҽ� �����Ѵ�.(�������)  */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	explicit CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CriticalSection() {
		return m_CriticalSection;
	}

	LEVEL Get_LevelIndex() {
		return m_eLevel;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoading;
	}

public:
	HRESULT NativeConstruct(LEVEL eLevel);

public:
	// ������ �ε� �Լ�
	HRESULT Loading_ForLoadingLevel();
	HRESULT Loading_ForGamePlayLevel();
	HRESULT Loading_ForEndingLevel();

private:
	// �ε� ���� �ε�
	HRESULT Loading_Loading_Texture();
	HRESULT Loading_Loading_Object();
	HRESULT Loading_Loading_Shader();
	HRESULT Loading_Loading_Model();

private:
	// ���� �÷��� ���� �ε�
	HRESULT Loading_GamePlay_Texture();
	HRESULT Loading_GamePlay_Effect_Texture();
	HRESULT Loading_GamePlay_Object();
	HRESULT Loading_GamePlay_Shader();
	HRESULT Loading_GamePlay_Model();
	HRESULT Loading_GamePlay_Component();

	// �ؽ��� - ������
	HRESULT Loading_GamePlay_Texture_Item();

	// �ڽ�Ʈ �� è�Ǿ�
	HRESULT Loading_GamePlay_Model_Champiom1Cost();
	HRESULT Loading_GamePlay_Model_Champiom2Cost();
	HRESULT Loading_GamePlay_Model_Champiom3Cost();
	HRESULT Loading_GamePlay_Model_Champiom4Cost();
	HRESULT Loading_GamePlay_Model_Champiom5Cost();

	// ĳ����
	HRESULT Loading_GamePlay_Model_Character();

	// ��
	HRESULT Loading_GamePlay_Model_MapObject_Personal();
	HRESULT Loading_GamePlay_Model_MapObject_Center();

	// �浹 ������Ʈ �ε�
	HRESULT Loading_GamePlay_Component_Collison();

	// �� ������Ʈ - ����ھ�(����ü)
	HRESULT Loading_GamePlay_Model_MapObject_Hexacore();

	// ������
	HRESULT Loading_GamePlay_Model_Item();

private:
	// ���� ���� �ε�
	HRESULT Loading_Ending_Texture();
	HRESULT Loading_Ending_Object();
	HRESULT Loading_Ending_Shader();
	HRESULT Loading_Ending_Model();

private:
	LEVEL			m_eLevel = LEVEL_END;
	_bool			m_isFinished = false;
	_tchar			m_szLoading[MAX_PATH] = TEXT("");

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	HANDLE				m_hThread = 0;
	CRITICAL_SECTION	m_CriticalSection;

public:
	static CLoader* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eLevel);
	virtual void Free() override;
};

END