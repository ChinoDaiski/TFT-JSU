#pragma once

#include "Base.h"

BEGIN(Engine)

class CRandom_Manager final : public CBase
{
	DECLARE_SINGLETON(CRandom_Manager)

public:
	CRandom_Manager();
	virtual ~CRandom_Manager() = default;

public:
	int		Get_RandomInt(int _iStart, int _iEnd);
	float	Get_RandomFloat(float _fStart, float _fEnd);

public:
	virtual void Free() override;
};

END