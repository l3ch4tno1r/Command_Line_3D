#pragma once

#include <string>

#include "Utilities/External/stb_image/stb_image.h"

namespace LCN
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const Texture&) = delete;
		Texture(Texture&& other);

		~Texture();

		void Load(const std::string& filepath, int deisredChannels);

		operator bool() const;

		uint8_t operator()(int i, int j) const;

		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other);

		int Width()  const;
		int Height() const;

	private:
		unsigned char* m_LocalBuffer = nullptr;

		int m_Width  = 0;
		int m_Height = 0;
		int m_BPP    = 0;
	};
}