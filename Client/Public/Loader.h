#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 레벨별로 로딩해야할 리소스 생성한다.(스레드로)  */

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
	// 레벨별 로딩 함수
	HRESULT Loading_ForLoadingLevel();
	HRESULT Loading_ForGamePlayLevel();
	HRESULT Loading_ForEndingLevel();

private:
	// 로딩 레벨 로드
	HRESULT Loading_Loading_Texture();
	HRESULT Loading_Loading_Object();
	HRESULT Loading_Loading_Shader();
	HRESULT Loading_Loading_Model();

private:
	// 게임 플레이 레벨 로딩
	HRESULT Loading_GamePlay_Texture();
	HRESULT Loading_GamePlay_Effect_Texture();
	HRESULT Loading_GamePlay_Object();
	HRESULT Loading_GamePlay_Shader();
	HRESULT Loading_GamePlay_Model();
	HRESULT Loading_GamePlay_Component();

	// 텍스쳐 - 아이템
	HRESULT Loading_GamePlay_Texture_Item();

	// 코스트 별 챔피언
	HRESULT Loading_GamePlay_Model_Champiom1Cost();
	HRESULT Loading_GamePlay_Model_Champiom2Cost();
	HRESULT Loading_GamePlay_Model_Champiom3Cost();
	HRESULT Loading_GamePlay_Model_Champiom4Cost();
	HRESULT Loading_GamePlay_Model_Champiom5Cost();

	// 캐릭터
	HRESULT Loading_GamePlay_Model_Character();

	// 맵
	HRESULT Loading_GamePlay_Model_MapObject_Personal();
	HRESULT Loading_GamePlay_Model_MapObject_Center();

	// 충돌 컴포넌트 로딩
	HRESULT Loading_GamePlay_Component_Collison();

	// 맵 오브젝트 - 헥사코어(증강체)
	HRESULT Loading_GamePlay_Model_MapObject_Hexacore();

	// 아이템
	HRESULT Loading_GamePlay_Model_Item();

private:
	// 엔딩 레벨 로딩
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