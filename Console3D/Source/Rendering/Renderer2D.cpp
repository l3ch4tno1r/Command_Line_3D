#include <thread>

#include "Renderer2D.h"

#include "Console3D/Source/Scene/StdComponent.h"

#include <Utilities/Source/Instrumentor.h>

namespace LCN::Render
{
    void Renderer2D::Init()
    {
        for (std::thread& thread : Get().m_ThreadPool)
            thread = std::thread();
    }

    void Renderer2D::Clear()
    {
        Core::Console::Get().Clear();
    }

    void Renderer2D::Submit()
    {
        Core::Console::Get().Render();
    }

    void Renderer2D::Render()
    {
        std::unique_lock<std::mutex> lock(Get().m_Mutex);

        Get().m_Condition.notify_all();
    }

    void Renderer2D::RenderWireFrame(const Scene& scene, Entity cameraEntity, const ViewPort& viewPort)
    {
        static Core::Console& console = Core::Console::Get();

        const Transform2Df R0ToCam = cameraEntity.Get<Component::Transform2DCmp>().Transform;

        Transform2Df CamToPix = cameraEntity.Get<Component::Camera2DCmp>().PixToCam.QuickInverse();
        Transform2Df PixToR0  = ((Transform2Df)(R0ToCam * CamToPix)).QuickInverse();

        auto view = scene.Registry().view<const Component::Transform2DCmp, const Component::Sprite2DCmp>();

        view.each([&](
            const Component::Transform2DCmp& R0ToSpriteCmp,
            const Component::Sprite2DCmp&    spriteCmp)
            {
                Transform2Df PixToSprite = PixToR0 * R0ToSpriteCmp.Transform;

                for (const auto& triangle : spriteCmp.Sprite.Faces)
                {
                    HVector2Df vertices[3];

                    // Compute vertices pos relative to cam
                    for (size_t i = 0; i < 3; ++i)
                    {
                        size_t vidx = triangle.v[i];

                        vertices[i] = PixToSprite * spriteCmp.Sprite.Vertices[vidx];
                    }

                    // Draw
                    for (size_t i = 0; i < 3; ++i)
                    {
                        const HVector2Df& vertex1 = vertices[i];
                        const HVector2Df& vertex2 = vertices[(i + 1) % 3];

                        console.DrawLine(
                            (int)vertex1.x(), (int)vertex1.y(),
                            (int)vertex2.x(), (int)vertex2.y());
                    }
                }
            });

        scene.Registry().view<const Component::Transform2DCmp, const Component::FrameCmp>().each([&](
            const Component::Transform2DCmp& R0ToFrame,
            const Component::FrameCmp&       frameCmp)
            {
                Transform2Df pixToFrame = PixToR0 * R0ToFrame.Transform;

                HVector2Df O = HVector2Df::Zero(); O.w() = 1.0f;
                HVector2Df X = frameCmp.Scale * HVector2Df::UnitX(); X.w() = 1.0f;
                HVector2Df Y = frameCmp.Scale * HVector2Df::UnitY(); Y.w() = 1.0f;

                HVector2Df _O = pixToFrame * O;
                HVector2Df _X = pixToFrame * X;
                HVector2Df _Y = pixToFrame * Y;

                console.DrawLine(
                    (int)_O.x(), (int)_O.y(),
                    (int)_X.x(), (int)_X.y(),
                    0, Core::COLOUR::BG_RED);

                console.DrawLine(
                    (int)_O.x(), (int)_O.y(),
                    (int)_Y.x(), (int)_Y.y(),
                    0, Core::COLOUR::BG_GREEN);
            });
    }

    void Renderer2D::RenderSequential(Scene& scene, Entity cameraEntity, const ViewPort& viewPort)
	{
        PROFILE_FUNC();

        Core::Console& console = Core::Console::Get();

        const Transform2Df& R0ToCam = cameraEntity.Get<Component::Transform2DCmp>().Transform;

        Transform2Df CamToPix = cameraEntity.Get<Component::Camera2DCmp>().PixToCam.QuickInverse();
        Transform2Df R0ToPix = R0ToCam * CamToPix;

        auto view = scene.Registry().view<Component::Transform2DCmp, Component::Sprite2DCmp, Component::TextureCmp>();

        console.FillRectangle(
            viewPort.TLx, viewPort.TLy,
            viewPort.BRx, viewPort.BRy,
            [&](int i, int j)
            {
                HVector2Df pixelFromPix = { (float)i, (float)j, 1.0f };

                CHAR_INFO result;

                float alpha0 = 1.0f;
                float color0 = 0.0f;

                result.Char.UnicodeChar = 0;
                result.Attributes = Core::COLOUR::BG_BLACK;

                view.each([&](
                    const Component::Transform2DCmp& R0ToSpriteCmp,
                    const Component::Sprite2DCmp&    spriteCmp,
                    const Component::TextureCmp&     textureCmp)
                    {
                        Transform2Df SpriteToPix = R0ToSpriteCmp.Transform.QuickInverse() * R0ToPix;

                        HVector2Df pixelFromSprite = SpriteToPix * pixelFromPix;

                        // TODO : Hard code to be remade ?
                        if (pixelFromSprite.x() < spriteCmp.Sprite.Vertices[0].x() ||
                            pixelFromSprite.x() > spriteCmp.Sprite.Vertices[2].x() ||
                            pixelFromSprite.y() > spriteCmp.Sprite.Vertices[0].y() ||
                            pixelFromSprite.y() < spriteCmp.Sprite.Vertices[2].y())
                            return;

                        static const char* value = " -.:*+=%#@";

                        // Find the texel coordinate
                        HVector2Df textureCoord = spriteCmp.SpriteToTexture * pixelFromSprite;

                        int tx = (textureCmp.Texture.Width() - 1) * std::min(std::max(textureCoord.x(), 0.0f), 1.0f);
                        int ty = (textureCmp.Texture.Height() - 1) * std::min(std::max(textureCoord.y(), 0.0f), 1.0f);

                        Ressource::TexelGreyScale texel = textureCmp.Texture(tx, ty);

                        // Alpha blending
                        float alpha = (float)texel.Alpha / 255;
                        float color = texel.GreyScale;

                        float temp = alpha + alpha0 * (1 - alpha);

                        color0 = (color * alpha + color0 * alpha0 * (1 - alpha)) / temp;
                        alpha0 = temp;

                        //uint8_t greyscale = 10 * (uint8_t)color0 / 255;
                        uint8_t greyscale = 9 * (uint8_t)color0 / 255;

                        //result.Char.UnicodeChar = greyscale == 10 ? 0 : value[greyscale];
                        //result.Attributes = greyscale == 10 ? COLOUR::BG_WHITE : COLOUR::FG_WHITE | COLOUR::BG_BLACK;
                        result.Char.UnicodeChar = value[greyscale];
                        result.Attributes = Core::COLOUR::FG_WHITE | Core::COLOUR::BG_BLACK;
                    });

                return result;
            });

        // Draw crosshair
        console.DrawLine(
            console.Width() / 2 - 5, console.Height() / 2,
            console.Width() / 2 + 5, console.Height() / 2,
            0, Core::COLOUR::BG_RED
        );

        console.DrawLine(
            console.Width() / 2, console.Height() / 2 - 5,
            console.Width() / 2, console.Height() / 2 + 5,
            0, Core::COLOUR::BG_RED
        );

        //console.Render();
	}

    void Renderer2D::RenderParallel(Scene& scene, Entity cameraEntity, const ViewPort& viewPort)
    {
        PROFILE_FUNC();

        int TLx = viewPort.TLx;
        int TLy = viewPort.TLy;
        int BRx = viewPort.BRx;
        int BRy = viewPort.BRy;

        ViewPort subViewPort[] = {
            { TLx,             TLy,             (TLx + BRx) / 2, (TLy + BRy) / 2 },
            { (TLx + BRx) / 2, TLy,             BRx,             (TLy + BRy) / 2 },
            { TLx,             (TLy + BRy) / 2, (TLx + BRx) / 2, BRy             },
            { (TLx + BRx) / 2, (TLy + BRy) / 2, BRx,             BRy             }
        };

        std::thread threads[] = {
            std::thread(Renderer2D::RenderSequential, std::ref(scene), cameraEntity, std::cref(subViewPort[0])),
            std::thread(Renderer2D::RenderSequential, std::ref(scene), cameraEntity, std::cref(subViewPort[1])),
            std::thread(Renderer2D::RenderSequential, std::ref(scene), cameraEntity, std::cref(subViewPort[2])),
            std::thread(Renderer2D::RenderSequential, std::ref(scene), cameraEntity, std::cref(subViewPort[3]))
        };

        for (auto& t : threads)
            t.join();

        Core::Console::Get().Render();
    }

    Renderer2D& Renderer2D::Get() noexcept
    {
        static Renderer2D renderer;
        return renderer;
    }

    Renderer2D::~Renderer2D()
    {

    }

    void Renderer2D::RenderThread()
    {

    }
}
