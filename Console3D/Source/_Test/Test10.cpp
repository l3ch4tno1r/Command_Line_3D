#ifdef TEST10

#include "Utilities\Utils.h"

bool LineInSight(HVector2D& OA, HVector2D& OB)
{
	static const HVector2D TL = { 0.0f,           0.0f };
	static const HVector2D TR = { (float)m_Width, 0.0f };
	static const HVector2D BL = { 0.0f,           (float)m_Height };
	static const HVector2D BR = { (float)m_Width, (float)m_Height };

	HVector2D AB = OB - OA;

	float t0y, tWy;
	float tx0, txH;

	if (std::abs(AB.x) > 0.01f)
	{
		t0y = OA.x / (AB.x);
	}

	return true;
}

int main()
{
	std::cin.get();
}

#endif // TEST10
