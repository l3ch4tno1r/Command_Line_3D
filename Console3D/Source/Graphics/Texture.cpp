#include "Texture.h"

#include "External/stb_image/stb_image.h"

Texture::~Texture()
{
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

	m_LocalBuffer = nullptr;
}

void Texture::Load(const std::string & filepath, int deisredChannels)
{
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, deisredChannels);
}

Texture::operator bool() const
{
	return m_LocalBuffer;
}

uint8_t Texture::operator()(int i, int j) const
{
	return m_LocalBuffer[i + j * m_Width];
}

int Texture::Width() const { return m_Width; }

int Texture::Height() const { return m_Height; }