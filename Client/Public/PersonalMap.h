#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Champion.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

/*
	�ʵ�� è�Ǿ��� ��ġ �� ������ �ó����� Ȱ�� ���θ� �����Ѵ�.
	���� ��ġ�� �����ϸ� �ش� è�Ǿ��� ������ �ִ� �ó����� ���ؼ��� �˻縦 �Ͽ� ���� ���θ� �����Ѵ�.
	
	Ȱ���� �ó����� �ʵ忡 �ִ� è�Ǿ𿡰� ��������ش�. è�Ǿ��� Ȱ���� �ó����� ���� ������ ������ �ִ�.
	
	���� ��ġ�� ���� ������ ���� ���� è�Ǿ��� 3������ �����ϸ�, 3������ ���ļ� �ٸ� ���� è�Ǿ����� �����Ѵ�.
	���ŵ� è�Ǿ��� �ɷ�ġ�� �ö󰡸�, ũ�Ⱑ Ŀ���� ����� ���ϸ�, ��ų�� ��� ���� �ö󰣴�. 


	�������� ũ���� ������ ������, �ʵ忡 ������. �ش� �����۵��� �÷��̾ ��ŷ���� ��ġ ����.
	��ġ ���н� ���� ������ �ִ� �ڸ��� ���ư�.
*/

class CPersonalMap final : public CGameObject
{
public:
	enum MAP_NAME {
		MAP_NAME_Celebrationtomorrow,
		MAP_NAME_Hexbattle,
		MAP_NAME_ZaunCity,
		MAP_NAME_Jinx,
		MAP_NAME_Vi,
		MAP_NAME_Spiritblossom_kami,
		MAP_NAME_Spiritblossom_reality,
		MAP_NAME_Spiritblossom_yokai,
		MAP_NAME_END
	};

	typedef struct _tagPersonalMapDesc {
		MAP_NAME	eType;				// ���� Ÿ��
		_float4x4	TransformMatrix;	// �̵� ���
	}PersonalMapDesc;

private:
	explicit CPersonalMap(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPersonalMap(const CPersonalMap& rhs);
	virtual ~CPersonalMap() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;

public:
	HRESULT SetUp_Components(void * pArg);
	HRESULT SetUp_ConstantTable();

public:
	static CPersonalMap* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	

public:
	MAP_NAME Get_Type(void) { return m_eMapName; }


public:
	void checkSynergy(void);	// Ȱ��ȭ�� �ó����� Ȯ��, è�Ǿ��� ���� ��ĥ�� �� ���� �ش� �Լ��� �ҷ� ���������� Ȱ��ȭ�� �ó����� ������ Ȯ���Ѵ�.
	void AssignChampion(CChampion* pChampion);		// �������� è�Ǿ��� ���� ��� ����Ǹ�, ���ڷ� �� è�Ǿ��� �޾� �ش� è�Ǿ��� �ʵ忡 ��ġ�ϴ� �Լ�

public:
	// �ó��� ���� �Լ�
	void Check_Rival();			// ������
	void Check_Scrap();			// ����
	void Check_Glutton();		// ��İ�
	void Check_Mutant();		// ��������
	void Check_Hextech();		// ��������
	void Check_Syndicate();		// ���� ����
	void Check_Socialite();		// �米��
	void Check_Clockwork();		// �ð��¿�
	void Check_Debonair();		// ���̺�
	void Check_Yordle();		// ���
	void Check_Yordle_Lord();	// ��� ����
	void Check_Mercenary();		// �뺴
	void Check_Enforcer();		// ������
	void Check_Chemtech();		// ȭ�а���
	void Check_Mastermind();	// �渷
	void Check_Enchanter();		// ��ȭ����
	void Check_Colossus();		// �Ž�
	void Check_Bodyguard();		// ��ȣ��
	void Check_Bruiser();		// ������
	void Check_Challenger();	// ������
	void Check_Transformer();	// ��������
	void Check_Arcanist();		// ����������
	void Check_Twinshot();		// �ֹ���
	void Check_Assassin();		// �ϻ���
	void Check_Sniper();		// ���ݼ�
	void Check_Striker();		// Ÿ�ݴ�
	void Check_Scholar();		// ����
	void Check_Innovator();		// ���Ű�

private:
	map<SYNERGY, int>	m_ChampionSynergy;		// �ʵ忡 ��ġ�� è�Ǿ��� �ó��� ����
	map<SYNERGY, int>	m_ActivatedSynergy;		// Ȱ��ȭ�� �ó����� ����

	list<CChampion*>	m_Champions;			// ���� �ʵ忡 �ִ� è�Ǿ��� ����

	MAP_NAME			m_eMapName;				// �� �ʵ��� �̸�. �ε�� ���� �̸��̱⵵ ��.
};

END