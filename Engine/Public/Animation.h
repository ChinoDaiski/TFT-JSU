#pragma once

#include "Base.h"
#include "Channel.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT NativeConstruct_Prototype(aiAnimation* pAIAnimation, vector<class CHierarchyNode*> Nodes);
	HRESULT NativeConstruct_Prototype(ifstream& In, vector<class CHierarchyNode*> Nodes);
	HRESULT NativeConstruct(aiAnimation* pAIAnimation, vector<class CHierarchyNode*> Nodes);
	HRESULT NativeConstruct(vector<class CHierarchyNode*> Nodes);

public:
	void Update(_double TimeDelta, _bool bCut);
	void Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint * pCurrentAnimIndex, _uint * pCurrentAnimIndexModel, vector<CChannel*> vChannel);

public:
	vector<CChannel*> Get_Channels();

public:
	HRESULT Save_AnimationInfo(ostream& Out);

private:
	char					m_szName[MAX_PATH] = "";
	_double					m_Duration = 0.0; /* 애니메이션 하나를 재생하는데 걸리는 총 시간. */
	_double					m_TickPerSecond = 0.0;

	_double					m_TimeAcc = 0.0; 
	_double					m_TimeAccNext = 0.0;
	_bool					m_isFinished = false;

private:
	_uint								m_iNumChannels = 0;

	vector<class CChannel*>				m_Channels;
	typedef vector<class CChannel*>		CHANNELS; 
	
private:
	_bool m_isCloned = false;

private:
	HRESULT Ready_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT Clone_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT Clone_Dat_Channels(vector<CHierarchyNode*> Nodes);

public:
	static CAnimation* Create(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	static CAnimation* Create(ifstream& In, vector<CHierarchyNode*> Nodes);
	CAnimation* Clone(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	CAnimation* Clone(vector<CHierarchyNode*> Nodes);
	virtual void Free() override;
};

END