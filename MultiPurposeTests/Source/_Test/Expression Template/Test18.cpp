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

	Vector3D(float x, float y, float z) :
		x(x),
		y(y),
		z(z)
	{
		std::cout << "Construct" << std::endl;
	}

	inline float& operator[](unsigned int i)
	{
		return mat[i];
	}

	inline float operator[](unsigned int i) const
	{
		return mat[i];
	}
};

template<typename LOp, typename ROp>
class AddVector3D
{
private:
	const LOp& lop;
	const ROp& rop;

public:
	AddVector3D(const LOp& _lop, const ROp& _rop) :
		lop(_lop),
		rop(_rop)
	{}

	inline float operator[](unsigned int i) const
	{
		return lop[i] + rop[i];
	}

	operator Vector3D() const
	{
		return {
			(*this)[0],
			(*this)[1],
			(*this)[2],
		};
	}
};

template<typename LOp, typename ROp>
AddVector3D<AddVector3D<LOp, ROp>, Vector3D> operator+(const AddVector3D<LOp, ROp>& lop, const Vector3D& vec)
{
	return AddVector3D<AddVector3D<LOp, ROp>, Vector3D>(lop, vec);
}

template<typename LOp, typename ROp>
AddVector3D<Vector3D, AddVector3D<LOp, ROp>> operator+(const Vector3D& lop, const AddVector3D<LOp, ROp>& vec)
{
	return AddVector3D<Vector3D, AddVector3D<LOp, ROp>>(lop, vec);
}

template<typename LLOp, typename LROp, typename RLOp, typename RROp>
AddVector3D<AddVector3D<LLOp, LROp>, AddVector3D<RLOp, RROp>> operator+(const AddVector3D<LLOp, LROp>& lop, const AddVector3D<RLOp, RROp>& vec)
{
	return AddVector3D<AddVector3D<LLOp, LROp>, AddVector3D<RLOp, RROp>>(lop, vec);
}

AddVector3D<Vector3D, Vector3D> operator+(const Vector3D& a, const Vector3D& b)
{
	return AddVector3D<Vector3D, Vector3D>(a, b);
}

int main()
{
	Vector3D a = {  1,  2,  3 };
	Vector3D b = {  4,  5,  6 };
	Vector3D c = {  7,  8,  9 };
	Vector3D d = { 10, 11, 12 };

	std::cout << "------------------" << std::endl;

	Vector3D e = (a + b) + (c + d);

	std::cin.get();
}