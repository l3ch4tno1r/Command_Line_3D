#pragma once

#include <exception>

#define ASSERT(X) if(!(X)) __debugbreak()

std::exception buildexception(const char* expr, const char* file, int line);

#define THROWEXCEPTIF(X) if(X) throw buildexception(#X, __FILE__, __LINE__);

#define BREACKIF(X) if(X) __debugbreak()