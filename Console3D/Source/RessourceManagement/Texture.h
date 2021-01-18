#pragma once

#include <string>

namespace LCN
{
	class Texture
	{
	public:
		~Texture();

		void Load(const std::string& filepath, int deisredChannels);

		operator bool() const;

		uint8_t operator()(int i, int j) const;

		int Width()  const;
		int Height() const;

	private:
		unsigned char* m_LocalBuffer = nullptr;

		int m_Width  = 0;
		int m_Height = 0;
		int m_BPP    = 0;
	};
}