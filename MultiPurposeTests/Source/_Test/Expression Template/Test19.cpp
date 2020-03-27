#include <iostream>

template<typename E>
class VecExpression
{
public:
	float operator[](unsigned int i) const
	{
		return static_cast<const E&>(*this)[i];
	}

	/*
	float x() const
	{
		return static_cast<const E&>(*this).x();
	}

	float y() const
	{
		return static_cast<const E&>(*this).y();
	}

	float z() const
	{
		return static_cast<const E&>(*this).z();
	}
	*/
};

struct Vector3D : public VecExpression<Vector3D>
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
	{}

	template<typename E>
	Vector3D(const VecExpression<E>& e)
	{
		for (unsigned int i = 0; i < 3; ++i)
			mat[i] = e[i];
	}

	float& operator[](unsigned int i)
	{
		return mat[i];
	}

	float operator[](unsigned int i) const
	{
		return mat[i];
	}
};

///////////////
//-- Somme --//
///////////////
template<typename EL, typename ER>
class VecSum : public VecExpression<VecSum<EL, ER>>
{
private:
	const EL& expl;
	const ER& expr;

public:
	VecSum(const EL& el, const ER& er) :
		expl(el),
		expr(er)
	{}

	float operator[](unsigned int i) const
	{
		return expl[i] + expr[i];
	}
};

template<typename EL, typename ER>
VecSum<EL, ER> operator+(const VecExpression<EL>& el, const VecExpression<ER>& er)
{
	return VecSum<EL, ER>(static_cast<const EL&>(el),static_cast<const ER&>(er));
}

////////////////////
//-- Différence --//
////////////////////
template<typename EL, typename ER>
class VecDif : public VecExpression<VecDif<EL, ER>>
{
private:
	const EL& expl;
	const ER& expr;

public:
	VecDif(const EL& el, const ER& er) :
		expl(el),
		expr(er)
	{}

	float operator[](unsigned int i) const
	{
		return expl[i] - expr[i];
	}
};

template<typename EL, typename ER>
VecDif<EL, ER> operator-(const VecExpression<EL>& el, const VecExpression<ER>& er)
{
	return VecDif<EL, ER>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}

////////////////////////////////////////
//-- Multiplication par un scalaire --//
////////////////////////////////////////
template<typename E>
class VecScaleMul : public VecExpression<VecScaleMul<E>>
{
private:
	float scalar;
	const E& e;

public:
	VecScaleMul(float scalar, const E& e) :
		scalar(scalar),
		e(e)
	{}

	float operator[](unsigned int i) const
	{
		return scalar * e[i];
	}
};

template<typename E>
VecScaleMul<E> operator*(float scalar, const VecExpression<E>& e)
{
	return VecScaleMul<E>(scalar, static_cast<const E&>(e));
}

int main()
{
	Vector3D a = { 1, 2, 3 };
	Vector3D b = { 4, 5, 6 };
	Vector3D c = { 7, 8, 9 };

	auto d = a + 2 * (b - c);

	std::cin.get();
}