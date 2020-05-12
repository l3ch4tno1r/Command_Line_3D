#ifdef TEST10

#include <vector>
#include <algorithm>

#include "Utilities\Utils.h"

const uint32_t m_Width  = 5;
const uint32_t m_Height = 3;

bool LineInSight(HVector2D& OA, HVector2D& OB)
{
	static const HVector2D TL = { 0.0f,           0.0f };
	static const HVector2D TR = { (float)m_Width, 0.0f };
	static const HVector2D BL = { 0.0f,           (float)m_Height };
	static const HVector2D BR = { (float)m_Width, (float)m_Height };
	
	HVector2D AB = OB - OA;
	HVector2D n  = { -AB.y, AB.x };

	float _1 = n | (OA - TL);
	float _2 = n | (OA - TR);
	float _3 = n | (OA - BL);
	float _4 = n | (OA - BR);

	if (sign(_1) == sign(_2) &&
		sign(_1) == sign(_3) &&
		sign(_1) == sign(_4))
		return false;

	HVector2D _OA = OA;
	HVector2D _OB = OB;

	float W = (float)m_Width;
	float H = (float)m_Height;

	float t0y = -1000000;
	float tWy =  1000000;
	float tx0 = -1000000;
	float txH =  1000000;

	if (std::abs(AB.x) > 0.01f)
	{
		t0y = -_OA.x / (AB.x);
		tWy = (W - _OA.x) / (AB.x);
	}

	if (std::abs(AB.y) > 0.01f)
	{
		tx0 = -_OA.y / (AB.y);
		txH = (H - _OA.y) / (AB.y);
	}

	std::vector<float> tab = { 0.0f, 1.0f, t0y, tWy, tx0, txH };
	std::sort(tab.begin(), tab.end());

	if (tab[2] >= 1.0f || tab[3] <= 0.0f)
		return false;

	OA = _OA + tab[2] * AB;
	OB = _OA + tab[3] * AB;

	return true;
}

int main()
{
	// In Sight
	//HVector2D OA = { 6.0f, 5.0f };
	//HVector2D OB = { 3.0f, 2.0f };

	// In Sight
	//HVector2D OA = { 1.0f, 2.0f };
	//HVector2D OB = { 4.0f, 1.0f };

	// Not In Sight
	//HVector2D OA = { 1.0f, 6.0f };
	//HVector2D OB = { 7.0f, 4.0f };
	
	// In Sight
	//HVector2D OA = { 1.0f, -2.0f };
	//HVector2D OB = { 4.0f,  6.0f };

	// Not In Sight
	//HVector2D OA = { 9.0f, 0.0f };
	//HVector2D OB = { 6.0f, 1.0f };
	
	// In Sight
	//HVector2D OA = { 6.0f,  2.0f };
	//HVector2D OB = { 2.0f, -1.0f };
	
	// In Sight
	HVector2D OA = { 4.0f, 6.0f };
	HVector2D OB = { 4.0f, 4.0f };

	std::cout << (LineInSight(OA, OB) ? "" : "Not ") << "In Sight !" << std::endl;

	std::cout << OA << std::endl;
	std::cout << OB << std::endl;

	std::cin.get();
}

#endif // TEST10
