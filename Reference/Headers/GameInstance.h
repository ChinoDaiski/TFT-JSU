#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "PIpeLine.h"
#include "Light_Manager.h"
#include "Random_Manager.h"
#include "Imgui_Manager.h"
#include "MousePicking.h"
#include "Frustum.h"
#include "Font_Manager.h"

#include "Layer.h"

/* 클라이언트에 보여줘야할 인터페이스를 보관하고. 보여준다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance : public CBase {

	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, CGraphic_Device::GRAPHICDESC GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	void Tick_Engine(_double TimeDelta);
	HRESULT Render_Engine();
	HRESULT Render_Begin(_float4 vClearColor);
	HRESULT Render_End();
	HRESULT Clear_LevelResource(_uint iLevelIndex);

public: /* For.Graphic_Device */


public: /* For.Input_Device */
	bool Get_DIKeyState(_uchar byKeyID);
	bool Get_DIKeyPreState(_uchar byKeyID);

	bool Get_DIMCurButtonClick(CInput_Device::MOUSEBUTTONSTATE eDIMBState);
	bool Get_DIMPreButtonClick(CInput_Device::MOUSEBUTTONSTATE eDIMBState);

	_long Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eDIMMState);

public: /* For.Timer_Manager */
	HRESULT Add_Timers(const _tchar* pTimerTag);
	_double Compute_TimeDelta(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);


public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

public: /* For.PipeLine */
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState);
	_vector Get_CamPosition();
	_float4 Get_CamPositionFloat4();

public: /*For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Lights(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);

public: /*For.Font_Manager */
	HRESULT Add_Fonts(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Fonts(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor);

public: /*For.Random_Manager */
	int		Get_RandomInt(int _iStart, int _iEnd);
	float	Get_RandomFloat(float _fStart, float _fEnd);

public: /*For.Imgui_Manager */
	void	OnOffImgui();
	HRESULT StartFrame(void);
	HRESULT RenderImgui(void);
	void InitWin32Imgui(HWND hWnd);
	bool WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public: /*For.Picking_Manager */
	_bool Picking(_float3* pVerticesPos, _fmatrix WorldMatrix, _vector* pOut);
	const CMousePicking::RAYDESC Get_vRayDesc(void);

public: /*For.Frustum */
	_bool isInFrustum_WorldSpace(_vector vWorldPos, _float fRange = 0.f);

private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CRandom_Manager*			m_pRandom_Manager = nullptr;
	CImgui_Manager*				m_pImgui_Manager = nullptr;
	CMousePicking*				m_pMousePicking_Manager = nullptr;
	CFrustum*					m_pFrustum = nullptr;
	CFont_Manager*				m_pFont_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END