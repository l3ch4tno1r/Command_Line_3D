#include "AppTestDisplayImage.h"

namespace LCN
{
	/////////////////////
	//-- ImageEntity --//
	/////////////////////

	ImageEntity::ImageEntity(Texture&& other) :
		m_Texture(std::move(other))
	{}

	const Texture& ImageEntity::GetTexture() const { return m_Texture; }

	/////////////////////////////
	//-- AppTestDisplayImage --//
	/////////////////////////////

	AppTestDisplayImage::AppTestDisplayImage() :
		m_Scene(300, 200)
	{}

	void AppTestDisplayImage::Run()
	{
		this->SignalStartup.Trigger();

		this->WaitQuit();

		this->SignalQuit.Trigger();
	}
}