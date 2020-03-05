#pragma once

template<typename T>
short sign(T a)
{
	return (a > T(0)) - (a < T(0));
}