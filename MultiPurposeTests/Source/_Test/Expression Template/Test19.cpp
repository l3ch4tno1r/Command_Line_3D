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
#pragma region Sum
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
#pragma endregion

////////////////////
//-- Différence --//
////////////////////
#pragma region Difference
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
#pragma endregion

///////////////////////////
//-- Produit vectoriel --//
///////////////////////////
#pragma region Cross_Product
template<typename EL, typename ER>
class VecCrossProduct : public VecExpression<VecCrossProduct<EL, ER>>
{
private:
	const EL& expl;
	const ER& expr;

public:
	VecCrossProduct(const EL& el, const ER& er) :
		expl(el),
		expr(er)
	{}

	float operator[](unsigned int i) const
	{
		unsigned int ip1 = (i + 1) % 3;
		unsigned int ip2 = (i + 2) % 3;		

		return (i % 2 == 0 ? 1 : -1) * (expl[ip1] * expr[ip2] - expl[ip2] * expr[ip1]);
	}
};

template<typename EL, typename ER>
VecCrossProduct<EL, ER> operator^(const VecExpression<EL>& el, const VecExpression<ER>& er)
{
	return VecCrossProduct<EL, ER>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}
#pragma endregion

////////////////////////////////////////
//-- Multiplication par un scalaire --//
////////////////////////////////////////
#pragma region Scalar_Multiplication
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
#pragma endregion

//////////////////////////////////
//-- Division par un scalaire --//
//////////////////////////////////
#pragma region Scalar_Division
template<typename E>
class VecScaleDiv : public VecExpression<VecScaleDiv<E>>
{
private:
	float scalar;
	const E& e;

public:
	VecScaleDiv(float scalar, const E& e) :
		scalar(scalar),
		e(e)
	{}

	float operator[](unsigned int i) const
	{
		return e[i] / scalar;
	}
};

template<typename E>
VecScaleDiv<E> operator/(const VecExpression<E>& e, float scalar)
{
	return VecScaleDiv<E>(scalar, static_cast<const E&>(e));
}
#pragma endregion

int main()
{
	Vector3D a = { 1, 2, 3 };
	Vector3D b = { 4, 5, 6 };
	Vector3D c = { 7, 8, 9 };

	auto d = (a + b - 2 * c) / 3;

	std::cout << '(' << d[0] << ", " << d[1] << ", " << d[2] << ')' << std::endl;

	Vector3D x = { 1, 0, 0 };
	Vector3D y = { 0, 1, 0 };
	Vector3D z = x ^ y;

	std::cout << '(' << z.x << ", " << z.y << ", " << z.z << ')' << std::endl;

	std::cin.get();
}