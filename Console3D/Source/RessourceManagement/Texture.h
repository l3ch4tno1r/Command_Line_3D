#pragma once

#include <string>

#include "Utilities/External/stb_image/stb_image.h"

namespace LCN::Ressource
{
	enum class ColorChannels
	{
		None,
		Grey       = STBI_grey,
		GreyAplpha = STBI_grey_alpha,
		RGB        = STBI_rgb,
		RGBAlpha   = STBI_rgb_alpha
	};

	enum class TexelType
	{
		Grey,
		RGB
	};

	template<TexelType>
	struct Texel;

	template<>
	struct Texel<TexelType::Grey>
	{
		uint8_t GreyScale;
		uint8_t Alpha;
	};

	using TexelGreyScale = Texel<TexelType::Grey>;

	/////////////////
	//-- Texture --//
	/////////////////

	class Texture
	{
	public:
		Texture();
		Texture(const Texture&) = delete;
		Texture(Texture&& other);

		~Texture();

		void Load(const std::string& filepath);

		operator bool() const;

		inline TexelGreyScale operator()(int i, int j) const { return m_LocalBuffer[i + j * m_Width]; }

		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other);

		int Width()  const;
		int Height() const;

	private:
		TexelGreyScale* m_LocalBuffer = nullptr;

		int m_Width  = 0;
		int m_Height = 0;
		int m_BPP    = 0;
	};
}