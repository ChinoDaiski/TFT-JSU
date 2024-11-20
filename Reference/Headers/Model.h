#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{ 
public:
	typedef struct tagMaterialTextureInfo {
	int iMaterialIndex = 0;
	int iAI_TEXTURE_TYPE = 0;
	char strFilePath[MAX_PATH];
}MATERIALTEXTURE;

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
public:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshContainer() const {
		return m_iNumMeshContainers;
	}

	void Set_AnimationIndex(_uint iAnimIndex) {
		m_iCurrentAnimIndex = iAnimIndex;
	}

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual HRESULT NativeConstruct_Prototype(const char* pModelDatFilePath, _fmatrix PivotMatrix);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	void Update(_double TimeDelta); 
	void Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint * pCurrentAnimIndex);
	HRESULT Render(class CShader* pShader, const char* pBoneMatricesName, _uint iMeshContainerIndex, _uint iPassIndex);
	HRESULT Bind_Material_OnShader(class CShader* pShader, aiTextureType eType, const char* pConstantName, _uint iMeshContainerIndex);

private:
	const aiScene*			m_pScene = nullptr;
	Assimp::Importer		m_Importer;

private:
	TYPE					m_eType = TYPE_END;
	_float4x4				m_PivotMatrix;

private:
	_uint									m_iNumMeshContainers = 0;
	vector<class CMeshContainer*>			m_MeshContainers;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;

private: 
	_uint									m_iNumMaterials = 0;
	vector<MODELMATERIAL>					m_Materials;
	typedef vector<MODELMATERIAL>			MATERIALS;

private:
	_uint									m_iNumAnimations = 0;
	_uint									m_iCurrentAnimIndex = 0;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;

private:
	_uint									m_iNumNodes = 0;
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

private:
	char*	m_pDatFilePath = nullptr;

private:
	HRESULT Ready_MeshContainers();
	HRESULT Clone_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Animations();
	HRESULT Clone_Animations();	
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent = nullptr, _uint iDepth = 0);

private:
	HRESULT Clone_Dat_HierarchyNodes();
	HRESULT Clone_Dat_MeshContainers();
	HRESULT Clone_Dat_Animations();

private:
	vector<class CHierarchyNode*>			m_ClonedHierarchyNodes;

private:
	_bool							m_isCloned = false;

private:
	HRESULT Save_FBX(ofstream& Out);
	HRESULT Load_FBX(ifstream& In);

private:
	HRESULT Load_VertexBuffer(ifstream& In);
	HRESULT Load_Animation(ifstream& In);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END