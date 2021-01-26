#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/RessourceManagement/Texture.h>
#include <Console3D/Source/Scene/Scene.h>
#include <Console3D/Source/Rendering/Camera2D.h>

namespace LCN
{
	/////////////////////
	//-- ImageEntity --//
	/////////////////////

	class ImageEntity : public Entity
	{
	public:
		ImageEntity(Texture&& other);

		const Texture& GetTexture() const;

	private:
		Texture   m_Texture;
		Vector2Df m_Vertices[4];
	};

	/////////////////////////////
	//-- AppTestDisplayImage --//
	/////////////////////////////

	class AppTestDisplayImage : public Application
	{
	public:
		AppTestDisplayImage();

		template<class F>
		using SignalApp = Signal<AppTestDisplayImage, F>;

		void Run() override;

	private:
		void Startup();

	private: // Signals
		SignalApp<void()> SignalStartup;
		SignalApp<void()> SignalQuit;

	private:
		Scene    m_Scene;
		Camera2D m_Camera;
	};
}

using LCNApp = LCN::AppTestDisplayImage;