#include <iostream>

struct Vector3D
{
	union
	{
		struct
		{
			float x, y, z;
		};

		float mat[3];
	};

	inline float& operator[](unsigned int i)
	{
		return mat[i];
	}

	inline const float& operator[](unsigned int i) const
	{
		return mat[i];
	}
};

template<typename LOp, typename ROp>
class AddExpression
{
protected:
	const LOp& lop;
	const ROp& rop;

public:
	AddExpression(const LOp& _lop, const ROp& _rop) :
		lop(_lop),
		rop(_rop)
	{}
};

class AddVector3D : public AddExpression<Vector3D, Vector3D>
{
public:
	operator Vector3D() const
	{
		Vector3D result = {
			lop.x + rop.x,
			lop.y + rop.y,
			lop.z + rop.z,
		};
	}
};

int main()
{

	std::cin.get();
}