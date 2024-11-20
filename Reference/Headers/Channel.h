#pragma once

#include "Base.h"

/* Ư�� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� �ִϸ��̼� ����. */
/* �ִϸ��̼� ���� �ϳ��� ���� ���� Channel�� ������ ������� �� �մ�. */

BEGIN(Engine)

class CChannel final :  public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;

public:
	void SetUp_HierarchyNodePtr(class CHierarchyNode* pNode) {
		m_pHierarchyNode = pNode;
		Safe_AddRef(m_pHierarchyNode);
	}

	void Set_CurrentKeyFrame(_uint iKeyFrame) {
		m_iCurrentKeyFrame = iKeyFrame;
	}

public:
	HRESULT NativeConstruct_Prototype(aiNodeAnim* pAIChannel);
	HRESULT NativeConstruct_Prototype(ifstream & In);
	HRESULT NativeConstruct();
	void Compute_TransformationMatrix(_double PlayTime);
	
public:
	HRESULT Save_ChannelInfo(ostream& Out);

public:
	_uint Get_CurrentKeyFrameIndex();
	KEYFRAME* Get_KeyFrame(_uint iKeyFrame);
	CHierarchyNode* Get_HierarchyNode();


private:
	char				m_szName[MAX_PATH] = "";

private:
	_uint							m_iNumKeyFrames = 0;
	_uint							m_iCurrentKeyFrame = 0;

	vector<KEYFRAME*>				m_KeyFrames;
	typedef vector<KEYFRAME*>		KEYFRAMES;

	class CHierarchyNode*			m_pHierarchyNode = nullptr;

private:
	_float4x4						m_TransformationMatrix;
	_bool							m_isCloned = false;

public:
	_uint Get_RelatedIndex(void) { return m_RelatedNodeIndex; }
	void Set_RelatedIndex(_uint iIndex) { m_RelatedNodeIndex = iIndex; }

private:
	_uint							m_RelatedNodeIndex = 0;

private:

	HRESULT Ready_KeyFrames(aiNodeAnim* pAIChannel);

public:
	static CChannel* Create(aiNodeAnim* pAIChannel);
	static CChannel* Create(ifstream& In);
	CChannel* Clone();
	virtual void Free() override;
};

END