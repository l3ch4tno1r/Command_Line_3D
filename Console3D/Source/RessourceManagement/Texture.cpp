#include "Texture.h"

namespace LCN
{
	Texture::Texture() :
		m_LocalBuffer(nullptr),
		m_Width(0),
		m_Height(0),
		m_BPP(0)
	{}

	Texture::Texture(Texture&& other) :
		m_LocalBuffer(other.m_LocalBuffer),
		m_Width(other.m_Width),
		m_Height(other.m_Height),
		m_BPP(other.m_BPP)
	{
		other.m_LocalBuffer = nullptr;

		other.m_Width  = 0;
		other.m_Height = 0;
		other.m_BPP    = 0;
	}

	Texture::~Texture()
	{
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);

		m_LocalBuffer = nullptr;
	}

	void Texture::Load(const std::string & filepath, int deisredChannels)
	{
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);

		m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, deisredChannels);
	}

	Texture::operator bool() const
	{
		return m_LocalBuffer;
	}

	Texture& Texture::operator=(Texture&& other)
	{
		if (this == &other)
			return *this;

		m_LocalBuffer = other.m_LocalBuffer;
		m_Width       = other.m_Width;
		m_Height      = other.m_Height;
		m_BPP         = other.m_BPP;

		other.m_LocalBuffer = nullptr;
		other.m_Width       = 0;
		other.m_Height      = 0;
		other.m_BPP         = 0;
	}

	int Texture::Width() const { return m_Width; }

	int Texture::Height() const { return m_Height; }
}