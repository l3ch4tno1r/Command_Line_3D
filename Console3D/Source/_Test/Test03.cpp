#ifdef TEST03

#include <iostream>

#include "Stack\SqrSMatrix.h"

template<unsigned int LC>
using Matrif33f = Matrix::SqrMatrix<float, LC>;

int main()
{
	Matrif33f<3> mat(true);

	std::cin.get();
}

#endif // TEST03
