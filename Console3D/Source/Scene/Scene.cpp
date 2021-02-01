#include <cmath>

#include "Scene.h"
#include "Entity.h"
#include "StdComponent.h"

#include "Console3D/Source/Core/Console.h"

namespace LCN
{
    Entity LCN::Scene::CreateEntity()
    {
        Entity result = { m_Registry.create(), this };

        result.Add<Transform2DComponent>();

        return result;
    }

    /*
    void Scene::Render(Entity camera)
    {
        Console& console = Console::Get();

        console.Clear();

        Camera2DComponent&    camCmp       = m_Registry.get<Camera2DComponent>(camera);
        Transform2DComponent& camTransform = m_Registry.get<Transform2DComponent>(camera);

        auto view = m_Registry.view<Transform2DComponent, Sprite2DComponent>();

        const Transform2Df& pix2cam = camCmp.PixToCam;

        Transform2Df cam2R0 = camTransform.Transform.QuickInverse();

        auto pix2R0 = pix2cam * cam2R0;

        view.each([&](const Transform2DComponent& R02sprite, const Sprite2DComponent& spriteCmp)
            {
                Transform2Df pix2sprite = pix2R0 * R02sprite.Transform;

                for (const Sprite2D::Triangle& face : spriteCmp.Sprite.Faces)
                {
                    HVector2Df v1 = pix2sprite * spriteCmp.Sprite.Vertices[face.v1];
                    HVector2Df v2 = pix2sprite * spriteCmp.Sprite.Vertices[face.v2];
                    HVector2Df v3 = pix2sprite * spriteCmp.Sprite.Vertices[face.v3];

                    console.DrawLine(
                        (int)v1.x(), (int)v1.y(),
                        (int)v2.x(), (int)v2.y());

                    console.DrawLine(
                        (int)v2.x(), (int)v2.y(),
                        (int)v3.x(), (int)v3.y());

                    console.DrawLine(
                        (int)v1.x(), (int)v1.y(),
                        (int)v3.x(), (int)v3.y());
                }
            });

        // Draw crosshair
        console.DrawLine(
            console.Width() / 2 - 5, console.Height() / 2,
            console.Width() / 2 + 5, console.Height() / 2,
            0, COLOUR::BG_RED
        );

        console.DrawLine(
            console.Width() / 2, console.Height() / 2 - 5,
            console.Width() / 2, console.Height() / 2 + 5,
            0, COLOUR::BG_RED
        );

        console.Render();
    }
    */

    void Scene::Render(Entity camera)
    {
        Console& console = Console::Get();

        const Transform2Df& R0ToCam  = camera.Get<Transform2DComponent>().Transform;

        Transform2Df CamToPix = camera.Get<Camera2DComponent>().PixToCam.QuickInverse();
        Transform2Df R0ToPix = R0ToCam * CamToPix;

        auto view = m_Registry.view<Transform2DComponent, Sprite2DComponent, TextureComponent>();

        console.FillRectangle(
            0, 0,
            console.Width(), console.Height(),
            [&](int i, int j)
            {
                HVector2Df pixelFromPix = { (float)i, (float)j, 1.0f };

                CHAR_INFO result;

                result.Char.UnicodeChar = 0;
                result.Attributes = COLOUR::BG_BLACK;

                view.each([&](
                    const Transform2DComponent& R0ToSpriteCmp,
                    const Sprite2DComponent&    spriteCmp,
                    const TextureComponent&     textureCmp)
                    {
                        Transform2Df SpriteToPix = R0ToSpriteCmp.Transform.QuickInverse() * R0ToPix;

                        HVector2Df pixelFromSprite = SpriteToPix * pixelFromPix;

                        // TODO : Hard code to be remade
                        if (pixelFromSprite.x() >= spriteCmp.Sprite.Vertices[0].x() &&
                            pixelFromSprite.x() <= spriteCmp.Sprite.Vertices[2].x() &&
                            pixelFromSprite.y() <= spriteCmp.Sprite.Vertices[0].y() &&
                            pixelFromSprite.y() >= spriteCmp.Sprite.Vertices[2].y())
                        {
                            static const char* value = " -.:*+=%#@";

                            // Quick fix
                            // TODO : find better
                            pixelFromSprite[2] = 1.0f;

                            HVector2Df textureCoord = spriteCmp.SpriteToTexture * pixelFromSprite;

                            int tx = (textureCmp.Texture.Width()  - 1) * std::min(std::max(textureCoord.x(), 0.0f), 1.0f);
                            int ty = (textureCmp.Texture.Height() - 1) * std::min(std::max(textureCoord.y(), 0.0f), 1.0f);

                            uint8_t greyscale = 9 * textureCmp.Texture(tx, ty) / 255;

                            result.Char.UnicodeChar = value[greyscale];
                            result.Attributes = COLOUR::FG_WHITE | COLOUR::BG_BLACK;
                        }
                    });

                return result;
            });

        // Draw crosshair
        console.DrawLine(
            console.Width() / 2 - 5, console.Height() / 2,
            console.Width() / 2 + 5, console.Height() / 2,
            0, COLOUR::BG_RED
        );

        console.DrawLine(
            console.Width() / 2, console.Height() / 2 - 5,
            console.Width() / 2, console.Height() / 2 + 5,
            0, COLOUR::BG_RED
        );

        console.Render();
    }
}
