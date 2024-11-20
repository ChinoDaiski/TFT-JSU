#include "..\Public\Random_Manager.h"

IMPLEMENT_SINGLETON(CRandom_Manager)

CRandom_Manager::CRandom_Manager()
{

}

int CRandom_Manager::Get_RandomInt(int _iStart, int _iEnd)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(_iStart, _iEnd);

	return dist(gen);
}

float CRandom_Manager::Get_RandomFloat(float _fStart, float _fEnd)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(_fStart, _fEnd);

	return dist(gen);
}

void CRandom_Manager::Free()
{
}
